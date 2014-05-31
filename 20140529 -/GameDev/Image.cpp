#include "Image.h"

Image::Image()
   : hBitmap(NULL)
   , clrTransparent(RGB(0,0,0))
   , bUseTransparent(false)
   , bUseAlpha(false), byteAlpha(127), bSrcAlpha(false)
   , bUseRotate(false), fAngle(0.f)
{}
Image::~Image()
{
   release();
}

Image* Image::clone()
{
   Image* pImage = new Image(*this);
   return pImage;
}
// 이미지 파일명으로 읽어들이고 그릴 때 지정된 부분만 그리도록 부분도 설정.
BOOL Image::Load(LPCTSTR szFileName, const Rect& rc)
{
   // 이미 읽어들인 이미지가 있다면...
   if (hBitmap)
      // 해제를 하고..
      release();

   // 지정된 이미지를 읽어들인다.
   hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
   if (hBitmap == NULL)
   {
      _checkSupportedFile(szFileName);
      DWORD dwError = ::GetLastError();
      ::ErrorMessageBox(dwError);

      return FALSE;
   }

   // 그려질 부분도 설정한다.
   rcSrc = rc;

   // rcDest 를 기본적으로 rcSrc 와 동일한 값을 가지돼 시작점은 0,0 으로 초기화.
   rcDest = rcSrc << Size(rcSrc.left, rc.top);

   return TRUE;
}
// 이미지 파일명만으로 이미지를 읽어들인다.
// 그리는 부분은 이미지 전체.
BOOL Image::Load(LPCTSTR szFileName)
{
   // 이미 읽어들인 이미지가 있다면...
   if (hBitmap)
      // 해제를 하고...
      release();

   // 지정된 이미지를 읽어들이고...
   hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
   if (hBitmap == NULL)
      return FALSE;

   // 따로 지정된 그릴 부분이 없으니 이미지 전체를 설정.
   BITMAP bm;
   ::GetObject(hBitmap, sizeof(BITMAP), &bm);
   rcSrc = Rect(Point(0,0), Point(bm.bmWidth, bm.bmHeight));

   // rcDest 를 기본적으로 rcSrc 와 동일한 값을 가지도록 초기화.
   rcDest = rcSrc;

   return TRUE;
}
void Image::release()
{
   // 이미지 핸들이 유효하다면...
   if (hBitmap)
   {
      // 해제한다..
      ::DeleteObject(hBitmap);
      hBitmap = NULL;
   }
}
// 그리기..
void Image::Draw(HDC hdc)
{
   HBITMAP hUseBitmap = NULL;
   HBITMAP hRotateBitmap = NULL;

   ///////////////////////////////////////
   // 회전시키면 이미지가 차지하는 영역의 크기가 달라진다.
   // 그 달라진 영역을 처리하기 위한 사각형크기를 선언.
   Rect rcSrcTemp = rcSrc;
   Rect rcDestTemp = rcDest;
   if (bUseRotate)
   {
      // 회전 행렬
      Matrix<float,2,2> rotMat;
      rotMat.m[0][0] = cos(fAngle*D2R);
      rotMat.m[0][1] = sin(fAngle*D2R);
      rotMat.m[1][0] = -sin(fAngle*D2R);
      rotMat.m[1][1] = cos(fAngle*D2R);

      // 이미지의 중심점 좌표
      Matrix<float,2,1> _c;
      _c.m[0][0] = float(rcSrcTemp.width()/2);
      _c.m[1][0] = float(rcSrcTemp.height()/2);

      // 회전 후 변경된 좌표
      Matrix<float,2,1> _rc = rotMat*_c;

      // 회전된 이미지가 그려질 영역의 크기.
      int cx = int(sqrt(float(rcSrc.width()*rcSrc.width() + rcSrc.height()*rcSrc.height())));
      int cy = cx;
      Size center(cx/2, cy/2);
      rcDestTemp = Rect(rcDestTemp.lefttop(), cx, cy);
      rcSrcTemp = Rect(Point(0,0), cx, cy);

      // 회전된 이미지를 그릴 때 필요한 HDC, HBITMAP 선언.
      HDC hRotateDC = ::CreateCompatibleDC(hdc);
      hRotateBitmap = ::CreateCompatibleBitmap(hdc, cx,cy);
      HBITMAP hOldRotateBitmap = ::Select(hRotateDC, hRotateBitmap);

      // 기본적으로 CreateCompatibleBitmap으로 생성된 비트맵은 검정색이라서
      // 투명처리할 색으로 칠한다.
      HBRUSH bgBrush = ::CreateSolidBrush(clrTransparent);
      HBRUSH bgOldBrush = ::Select(hRotateDC, bgBrush);

      ::FillRect(hRotateDC, &rcSrcTemp, bgBrush);

      ::Select(hRotateDC, bgOldBrush);
      ::DeleteObject(bgBrush);

      // 회전 변환 시킬 XFORM 선언.
      XFORM xf;
      xf.eM11 = cos(fAngle*D2R);
      xf.eM12 = -sin(fAngle*D2R);
      xf.eM21 = sin(fAngle*D2R);
      xf.eM22 = cos(fAngle*D2R);
      xf.eDx = FLOAT(center.cx -_rc.m[0][0]);
      xf.eDy = FLOAT(center.cy -_rc.m[1][0]);

      // 변환을 하고..
      ::SetGraphicsMode(hRotateDC, GM_ADVANCED);
      ::SetWorldTransform(hRotateDC, &xf);

      HDC hBitmapDC = ::CreateCompatibleDC(hdc);
      HBITMAP hOldBitmap = ::Select(hBitmapDC, hBitmap);

      // 원래 이미지를 변환이 된 영역으로 복사.
      ::BitBlt(hRotateDC, 
         0,0,
         cx, cy,
         hBitmapDC, 
         rcSrc.left, rcSrc.top,
         SRCCOPY);

      // 변환된 이미지를 빼고 다른 사용했던 핸들들은 제거 및 해제.
      ::Select(hBitmapDC, hOldBitmap);
      ::DeleteDC(hBitmapDC);
      ::Select(hRotateDC, hOldRotateBitmap);
      ::DeleteDC(hRotateDC);

      hUseBitmap = hRotateBitmap;
   }
   else
   {
      hUseBitmap = hBitmap;
   }

   HDC hBitmapDC = ::CreateCompatibleDC(hdc);
   HBITMAP hOldBitmap = Select(hBitmapDC, hUseBitmap);

   // 회전 할때는 기본적으로 GdiTransparentBlt를 이용하자.
   if (bUseRotate)
   {
      ::GdiTransparentBlt(hdc,							// 그려질 DC
         rcDestTemp.left, rcDestTemp.top,			// 그려질 위치
         rcDestTemp.width(), rcDestTemp.height(),	// 그려질 크기
         hBitmapDC,									// 이미지 DC
         rcSrcTemp.left, rcSrcTemp.top,				// 이미지 부분 위치
         rcSrcTemp.width(), rcSrcTemp.height(),
         clrTransparent);							// 이미지를 그리는 방법.
   }
   // 투명처리를 사용한다면...
   else if (bUseAlpha)
   {
      BLENDFUNCTION bf;
      bf.BlendOp =  AC_SRC_OVER;
      bf.BlendFlags = 0;
      if (bSrcAlpha)
      {
         bf.AlphaFormat = AC_SRC_ALPHA;
         bf.SourceConstantAlpha = 0xFF;	// 0(완전투명:transparent) ~ 255(완전불투명:opaque)
      }
      else
      {
         bf.AlphaFormat = 0;
         bf.SourceConstantAlpha = byteAlpha;	// 0(완전투명:transparent) ~ 255(완전불투명:opaque)
      }

      ::GdiAlphaBlend(hdc, rcDest.left, rcDest.top,
         rcDest.width(), rcDest.height(),
         hBitmapDC,
         rcSrc.left, rcSrc.top,
         rcSrc.width(), rcSrc.height(),
         bf);
   }
   else if (bUseTransparent)
   {
      ::GdiTransparentBlt(hdc,									// 그려질 DC
         rcDest.left, rcDest.top,				// 그려질 위치
         rcDest.width(), rcDest.height(),		// 그려질 크기
         hBitmapDC,								// 이미지 DC
         rcSrc.left, rcSrc.top,			// 이미지 부분 위치
         rcSrc.width(), rcSrc.height(),	// 이미지 부분 크기
         clrTransparent);						// 투명처리할 색
   }
   else
   {
      //::BitBlt(hdc,								// 그려질 DC
      //   rcDest.left, rcDest.top,			// 그려질 위치
      //   rcDest.width(), rcDest.height(),	// 그려질 크기
      //   hBitmapDC,							// 이미지 DC
      //   rcSrc.left, rcSrc.top,				// 이미지 부분 위치
      //   SRCCOPY);							// 이미지를 그리는 방법.


	   ::StretchBlt(hdc,
		   rcDest.left,rcDest.top,
			rcDest.width(),rcDest.height(),
			hBitmapDC,
			rcSrc.left,rcSrc.top,
			rcSrc.right,rcSrc.bottom,
			SRCCOPY);
   }

   Select(hBitmapDC, hOldBitmap);
   ::DeleteDC(hBitmapDC);

   // 회전사용시에만 회전된 이미지를 제거한다.
   if (bUseRotate)
   {
      ::DeleteObject(hRotateBitmap);
   }
}
// 투명처리를 하기 위한 설정..
void Image::SetTransparent(COLORREF clr)
{
   clrTransparent = clr;
   bUseTransparent = true;
}
// 알파블랜딩으로 그리기 위한 설정.
void Image::SetAlpha(BYTE alpha, bool _bSrcAlpha/*=false*/)
{
   bUseAlpha = true;
   byteAlpha = alpha;
   bSrcAlpha = _bSrcAlpha;
}
// 그려질 부분의 크기를 반환.
Rect Image::getRect()
{
   return rcSrc;
}
COLORREF Image::getTransparentColor() const
{
   return clrTransparent;
}
void Image::SetRect(const Rect& rc)
{
	rcDest = rc;
}
void Image::Move(const Rect& rc)
{
   rcDest = rc;
}
void Image::Move(const Point& pt)
{
   rcDest = Rect(pt, Size(rcDest.width(), rcDest.height()));
}
void Image::Rotate(const float& angle)
{
   bUseRotate = true;
   fAngle = angle;
}
void Image::getClient(Rect rc)
{
	rcClient = rc;
}
// 이미지 핸들 자체를 필요로 하는 경우에 호출됨.
Image::operator HBITMAP ()
{
   return hBitmap;
}
// getRect 를 편하게 쓰기 위해 선언...
Image::operator Rect ()
{
   return rcSrc;
}
void Image::_checkSupportedFile(LPCTSTR lpszFileName)
{
   BITMAPFILEHEADER bfh;

   std::fstream bmp(lpszFileName, std::ios_base::in | std::ios_base::binary);
   if (!bmp.good())
   {
      ::MessageBox(NULL, _T("File Open Failed."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   bmp.read((char*)&bfh, sizeof(bfh));
   if (::memcmp((void*)&bfh.bfType, "BM", 2) != 0)
   {
      ::MessageBox(NULL, _T("Not Supported File."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   DWORD dwSize;

   bmp.read((char*)&dwSize, sizeof(DWORD));
   /*
   12 : BITMAPCOREHEADER or OS21XBITMAPHEADER 
   64 : OS22XBITMAPHEADER (x)
   40 : BITMAPINFOHEADER
   52 : BITMAPV2INFOHEADER (x)
   56 : BITMAPV3INFOHEADER (x)
   108 : BITMAPV4HEADER
   124 : BITMAPV5HEADER
   */
   if (dwSize == 64 || dwSize == 52 || dwSize == 56)
   {
      ::MessageBox(NULL, _T("DIB Header is not Supported."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   bmp.close();
}