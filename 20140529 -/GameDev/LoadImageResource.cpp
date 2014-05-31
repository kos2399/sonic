#include "LoadImageResource.h"

using namespace std;

LoadImageRes::LoadImageRes()
: szFileName(_T("Resource\\image.image"))
{
}
LoadImageRes::~LoadImageRes()
{
}

void LoadImageRes::Load(LPCTSTR lpszFileName/*=NULL*/)
{
	fstream file;
	if (lpszFileName)
		file.open(lpszFileName, ios_base::in);
	else
		file.open(szFileName, ios_base::in);

	while (file.good())
	{
		string buffer;
		getline(file, buffer);

		if (buffer.empty())
			continue;

		if (buffer[0] == '/')
			continue;

		string::size_type pos = buffer.find(' ');
		string::size_type prev = 0;

		// 데이터를 크게 4부분으로 처리..
		string id = buffer.substr(prev, pos-prev);
		prev = pos;
		pos = buffer.find(' ', prev+1);
		string cfilename = buffer.substr(prev+1, pos-prev-1);
		prev = pos;
		pos = buffer.find(' ', prev+1);
		string rect = buffer.substr(prev+1, pos-prev-1);
		prev = pos;
		pos = buffer.find(' ', prev+1);
		string option = buffer.substr(prev+1);

		if (id.empty() || cfilename.empty() || rect.empty())
			continue;

		// filename
		wstring wfilename;
		string::iterator it;
		for (it = cfilename.begin(); it != cfilename.end(); it++)
		{
			char c = *it;
			wchar_t wc = wchar_t(c);
			wfilename.append(1, wc);
		}

		// rect
		Rect rc;
		prev = 0;
		//찾을 문자열 값을 int로 리턴 0부터
		pos = rect.find(',',prev+1); 
		// atoi는 문자열을 정수로 변환한다.
		//c_str()는 string형 문자열을 char*형으로 바꿔준다.
		//substr(위치, 길이);
		rc.left	= atoi(rect.substr(prev, pos).c_str());  // 1,259,128,386

		prev = pos;  //1
		pos = rect.find(',',prev+1); //2부터 탐색.
		rc.top	= atoi(rect.substr(prev+1, pos).c_str());

		prev = pos;
		pos = rect.find(',',prev+1);
		rc.right = atoi(rect.substr(prev+1, pos).c_str());

		prev = pos;
		pos = rect.find(',',prev+1);
		rc.bottom = atoi(rect.substr(prev+1, pos).c_str());


		Image* pImage = new Image;
		pImage->Load(wfilename.c_str(), rc);

		// option
		if (!option.empty())
		{
			prev = 0;
			pos = option.find(' ', prev);
			int bTrans = atoi(option.substr(prev, pos-prev).c_str());
			if (bTrans == 1)
			{
				// 투명처리할 색 읽어오기...
				prev = pos;
				pos = option.find(' ', prev+1);
				string szColor = option.substr(prev+1, pos-prev-1);

				//BYTE r, g, b;
				//r = c2d(szColor[0])*16 + c2d(szColor[1]);
				//g = c2d(szColor[2])*16 + c2d(szColor[3]);
				//b = c2d(szColor[4])*16 + c2d(szColor[5]);

            pImage->SetTransparent(Color(szColor.c_str()));
			}
			prev = pos;
			pos = option.find(' ', prev+1);
			int bAlpha = atoi(option.substr(prev, pos-prev).c_str());
			if (bAlpha == 1)
			{
				// 알파값 읽어오기...
				prev = pos;
				pos = option.find(' ', prev+1);
				int nAlpha = atoi(option.substr(prev, pos-prev).c_str());

				prev = pos;
				pos = option.find(' ', prev+1);
				int nSrcAlpha = atoi(option.substr(prev).c_str());

				if (nSrcAlpha == 0)
					pImage->SetAlpha(BYTE(nAlpha), false);
				else
					pImage->SetAlpha(BYTE(nAlpha), true);
			}

		} // if (!option.empty())

		// 동일한 ID로 등록된 이미지가 있다면, 동적할당한 것 삭제한다.
		if (!ImgDepot.push(id.c_str(), pImage))
			delete pImage;

	} // while (file.good())

	file.close();
}

void LoadImageRes::operator ()(LPCTSTR lpszFileName/*=NULL*/)
{
	Load(lpszFileName);
}
