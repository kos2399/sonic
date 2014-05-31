#pragma once

#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include "Utility.hpp"
#include "BaseType.h"
#include "Sound.h"

class LoadSoundRes : public singleton<LoadSoundRes>
{
	friend class singleton<LoadSoundRes>;
private :
	LoadSoundRes();
	~LoadSoundRes();

public :
	void Load(LPCTSTR lpszFileName = NULL);
	void operator ()(LPCTSTR lpszFileName = NULL);

private :
	LPCTSTR szFileName;
};

#define SndLoader LoadSoundRes::getReference()