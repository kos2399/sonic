#pragma once

#include <windows.h>
#include <tchar.h>

template<typename T>
class singleton
{
protected :
	singleton(){::InitializeCriticalSection(&cs);}
	virtual ~singleton(){::DeleteCriticalSection(&cs);}

public :
	static T& getReference()
	{
		static T inst;
		return inst;
	}

protected :
	CRITICAL_SECTION cs;
};

template<typename GDIObject>
GDIObject Select(HDC hdc, GDIObject obj)
{
	return (GDIObject)::SelectObject(hdc, obj);
}

template<typename T>
void SafeDelete(T& pointer)
{
	if (pointer)
	{
		delete [] pointer;
		pointer = NULL;
	}
}

template<typename CharType>
CharType* strAlloc(CharType* & dest, const CharType* const src)
{
	SafeDelete(dest);

	unsigned int i = 0;
	while (src[i++]);

	dest = new CharType[i];

	i = 0;
	while (dest[i++] = src[i]);

	return dest;
}

void ErrorMessageBox(const DWORD& dwError);