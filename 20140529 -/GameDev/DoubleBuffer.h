#pragma once

#include <windows.h>
#include "BaseType.h"
#include "Utility.hpp"

class DoubleBuffer
{
	typedef DoubleBuffer Me;
public :
	DoubleBuffer();
	~DoubleBuffer();

	void Attach(HWND hWnd);
	void Detach();

	void Draw();

	Me& operator << (const COLORREF& color);
	operator HDC ();

	
private :
	HWND hOwner;
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	HBITMAP hOldMemBitmap;
	Rect rcClient;
};