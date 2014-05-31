#pragma once

#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include "Utility.hpp"
#include "BaseType.h"
#include "Image.h"

class LoadImageRes : public singleton<LoadImageRes>
{
	friend class singleton<LoadImageRes>;
private :
	LoadImageRes();
	~LoadImageRes();

public :
	void Load(LPCTSTR lpszFileName = NULL);
	void operator ()(LPCTSTR lpszFileName = NULL);

private :
	LPCTSTR szFileName;
};

#define ImgLoader LoadImageRes::getReference()