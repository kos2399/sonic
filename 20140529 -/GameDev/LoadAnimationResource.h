#pragma once

#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include "Utility.hpp"
#include "BaseType.h"
#include "Animation.h"

class LoadAnimationRes : public singleton<LoadAnimationRes>
{
	friend class singleton<LoadAnimationRes>;
private :
	LoadAnimationRes();
	~LoadAnimationRes();

public :
	void Load(LPCTSTR lpszFileName = NULL);
	void operator ()(LPCTSTR lpszFileName = NULL);

private :
	LPCTSTR szFileName;
};

#define AniLoader LoadAnimationRes::getReference()