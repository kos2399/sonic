#include "LoadSoundResource.h"

using namespace std;

LoadSoundRes::LoadSoundRes()
: szFileName(_T("Resource\\sound.sound"))
{
}
LoadSoundRes::~LoadSoundRes()
{
}

void LoadSoundRes::Load(LPCTSTR lpszFileName/*=NULL*/)
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

		// 데이터를 크게 3부분으로 처리..
		string id = buffer.substr(prev, pos-prev);
		prev = pos;
		pos = buffer.find(' ', prev+1);
		string cfilename = buffer.substr(prev+1, pos-prev-1);
		prev = pos;
		pos = buffer.find(' ', prev+1);
		string option = buffer.substr(prev+1);

		if (id.empty() || cfilename.empty())
			continue;

		Sound* pSound = new Sound;
		pSound->Load(cfilename.c_str());

		// option
		if (!option.empty())
		{
			prev = 0;
			pos = option.find(' ', prev);
			int bLoop = atoi(option.substr(prev, pos-prev).c_str());
			if (bLoop == 1)
			{
				pSound->SetLoop(true);
			}
			prev = pos;
			pos = option.find(' ', prev+1);
			int bLoopRange = atoi(option.substr(prev, pos-prev).c_str());
			if (bLoopRange == 1)
			{
				// 알파값 읽어오기...
				prev = pos;
				pos = option.find(' ', prev+1);
				int nFrom = atoi(option.substr(prev, pos-prev).c_str());
				prev = pos;
				pos = option.find(' ', prev+1);
				int nTo = atoi(option.substr(prev).c_str());

				pSound->SetLoopRange(unsigned(nFrom), unsigned(nTo));
			}

		}

		if (!SndDepot.push(id.c_str(), pSound))
			delete pSound;
	}

	file.close();
}

void LoadSoundRes::operator ()(LPCTSTR lpszFileName/*=NULL*/)
{
	Load(lpszFileName);
}
