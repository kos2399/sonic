#pragma once

#include <windows.h>
#include <bitset>
#include <vector>
#include "utility.hpp"
#include "BaseType.h"

class InputProcessor : public singleton<InputProcessor>
{
	friend class singleton<InputProcessor>;

	enum {count=256};

public :
	typedef std::bitset<count> checktype;
	typedef std::vector<int> KeyList;
private :
	InputProcessor();
	~InputProcessor();

public :
	void Update(DWORD tick);
	bool AnyKey() const;
	bool OnClick(const int& vkey);
   void Processed(const int& vkey);
	bool IsPressed(const int& vkey);
	bool operator [] (const int& vkey);
	Point pos() const;
	KeyList& current();

private :
	void _check();
private :
	BYTE pKey[count];
	BYTE cKey[count];

	checktype clickbits;
	KeyList pressedlist;
};

#define InputDevice InputProcessor::getReference()