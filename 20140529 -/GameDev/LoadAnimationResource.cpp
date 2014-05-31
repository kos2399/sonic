#include "LoadAnimationResource.h"

using namespace std;

LoadAnimationRes::LoadAnimationRes()
: szFileName(_T("Resource\\animation.animation"))
{
}
LoadAnimationRes::~LoadAnimationRes()
{
}

void LoadAnimationRes::Load(LPCTSTR lpszFileName/*=NULL*/)
{
	fstream file;
	if (lpszFileName)
		file.open(lpszFileName, ios_base::in);
	else
		file.open(szFileName, ios_base::in);

	Animation* pAni = NULL;
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
		int count = atoi(buffer.substr(prev+1, pos-prev-1).c_str());
		prev = pos;
		pos = buffer.find(' ', prev+1);
		int nLoop = atoi(buffer.substr(prev+1, pos-prev-1).c_str());
		prev = pos;
		pos = buffer.find(' ', prev+1);
		int nDelay = atoi(buffer.substr(prev+1).c_str());

		if (id.empty())
			continue;

		pAni = new Animation;

		pAni->SetDelay(nDelay);
		if (nLoop == 0)
			pAni->SetLoop(false);
		else
			pAni->SetLoop(true);

		for (int i = 0; i < count;)
		{
			getline(file, buffer);

			if (buffer.empty())
				continue;

			if (buffer[0] == '/')
				continue;

			prev = 0;
			pos = buffer.find(' ');


			string cfilename = buffer.substr(prev, pos-prev);
			prev = pos;
			pos = buffer.find(' ', prev+1);
			string rect = buffer.substr(prev+1, pos-prev-1);
			prev = pos;
			pos = buffer.find(' ', prev+1);
			string option = buffer.substr(prev+1);

			if (cfilename.empty() || rect.empty())
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
			pos = rect.find(',',prev+1);

			rc.left	= atoi(rect.substr(prev, pos).c_str());
			prev = pos;
			pos = rect.find(',',prev+1);
			rc.top	= atoi(rect.substr(prev+1, pos).c_str());
			prev = pos;
			pos = rect.find(',',prev+1);
			rc.right = atoi(rect.substr(prev+1, pos).c_str());
			prev = pos;
			pos = rect.find(',',prev+1);
			rc.bottom = atoi(rect.substr(prev+1, pos).c_str());


			Image* pImage = new Image;
			pImage->Load(wfilename.c_str(), rc);

			pAni->AddImage(pImage);

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

					pAni->SetTransparent(Color(szColor.c_str()));
				}
				prev = pos;
				pos = option.find(' ', prev+1);
				int bAlpha = atoi(option.substr(prev, pos-prev).c_str());
				if (bAlpha == 1)
				{
					// 알파값 읽어오기...
					prev = pos;
					pos = option.find(' ', prev+1);
					int nAlpha = atoi(option.substr(prev).c_str());

					pAni->SetAlpha(BYTE(nAlpha));
				} // if (bAlpha == 1)

			} // if (!option.empty())

			i++;
		} // for (int i = 0; i < count;)

		// 동일한 ID로 등록된 애니메이션이 있다면, 동적할당한 것 삭제처리.
		if (!AniDepot.push(id.c_str(), pAni))
			delete pAni;

	}

	file.close();
}

void LoadAnimationRes::operator ()(LPCTSTR lpszFileName/*=NULL*/)
{
	Load(lpszFileName);
}
