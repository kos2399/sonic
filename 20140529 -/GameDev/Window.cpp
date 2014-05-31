#include "Window.h"

Window::Window()
   : szTitle(NULL), bordersize(5L), fontHeight(30)
   , bGrabWindow(false), bPressed(false)
{
   clrArea[AREA_NORMAL] = Color("FFFFFF");
   clrArea[AREA_TITLE] = Color("AAAAAA");
   clrArea[AREA_TITLE_TEXT] = Color("000000");
   clrArea[AREA_BORDER] = Color("777777");
}
Window::~Window()
{
   ::SafeDelete(szTitle);
}

void Window::Input(DWORD tick)
{
   ptPrevMouse = ptMouse;
   ptMouse = InputDevice.pos();
   ptMouse = ptMouse.ToClient(hOwner);

   bPrevPressed = bPressed;

   if (InputDevice[VK_LBUTTON])
      bPressed = true;
   else
      bPressed = false;

   if (bPressed && !bPrevPressed && ::PtInRect(&rcTitle, ptMouse))
   {
      bGrabWindow = true;
   }
   else if (!bPressed)
   {
      bGrabWindow = false;
   }

   if (InputDevice.OnClick(VK_ESCAPE))
   {
      ShowControl(false);
      InputDevice.Processed(VK_ESCAPE);
   }
}
void Window::Update(DWORD tick)
{
   if (bGrabWindow)
   {
      Point diff = ptMouse - ptPrevMouse;

      SetDrawRect(rcDraw>>Size(diff.x,diff.y));
   }
}
void Window::Draw(HDC hdc)
{
   // AREA_BORDER
   HBRUSH hBrush = ::CreateSolidBrush(clrArea[AREA_BORDER]);
   HBRUSH hOldBrush = ::Select(hdc, hBrush);

   HPEN hPen = ::CreatePen(PS_SOLID, 1, clrArea[AREA_BORDER]);
   HPEN hOldPen = ::Select(hdc, hPen);

   //::RoundRect(hdc, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom, 20, 20);
   ::Rectangle(hdc, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

   ::Select(hdc, hOldPen);
   ::DeleteObject(hPen);

   ::Select(hdc, hOldBrush);
   ::DeleteObject(hBrush);

   // AREA_TITLE
   hBrush = ::CreateSolidBrush(clrArea[AREA_TITLE]);
   hOldBrush = ::Select(hdc, hBrush);

   hPen = ::CreatePen(PS_SOLID, 1, clrArea[AREA_TITLE]);
   hOldPen = ::Select(hdc, hPen);

   ::Rectangle(hdc, rcTitle.left, rcTitle.top, rcTitle.right, rcTitle.bottom);

   ::Select(hdc, hOldPen);
   ::DeleteObject(hPen);

   ::Select(hdc, hOldBrush);
   ::DeleteObject(hBrush);

   // AREA_NORMAL
   hBrush = ::CreateSolidBrush(clrArea[AREA_NORMAL]);
   hOldBrush = ::Select(hdc, hBrush);

   hPen = ::CreatePen(PS_SOLID, 1, clrArea[AREA_NORMAL]);
   hOldPen = ::Select(hdc, hPen);

   ::Rectangle(hdc, rcArea.left, rcArea.top, rcArea.right, rcArea.bottom);

   ::Select(hdc, hOldPen);
   ::DeleteObject(hPen);

   ::Select(hdc, hOldBrush);
   ::DeleteObject(hBrush);

	if (szTitle)
	{
		COLORREF clrOld = ::SetTextColor(hdc, clrArea[AREA_TITLE_TEXT]);

		int oldbkmode = ::GetBkMode(hdc);
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, szTitle, -1, &rcTitle, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		::SetBkMode(hdc, oldbkmode);

		::SetTextColor(hdc, clrOld);
	}

}

void Window::SetDrawRect(const Rect& rc)
{
   rcDraw = rc;

   rcTitle = rcDraw - Size(bordersize, bordersize);
   rcTitle.bottom = rcTitle.top + fontHeight;

   rcArea = rcDraw - Size(bordersize, bordersize);
   rcArea.top = rcTitle.bottom;
}
void Window::SetWindowTitle(LPCTSTR title, const Color& clr/*=Color("000000")*/)
{
   ::strAlloc(szTitle, title);

   clrArea[AREA_TITLE_TEXT] = clr;
}
void Window::SetWindowColor(const Color& title, const Color& border, const Color& area)
{
   clrArea[AREA_NORMAL] = area;
   clrArea[AREA_TITLE] = title;
   clrArea[AREA_BORDER] = border;
}
