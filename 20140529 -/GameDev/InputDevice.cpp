#include "InputDevice.h"

InputProcessor::InputProcessor()
{
	::memset(cKey, 0, sizeof(BYTE)*count);
}
InputProcessor::~InputProcessor()
{
}

void InputProcessor::Update(DWORD tick)
{
	// 가지고 있던 키상태를 이전상태로 복사한 후
	::memcpy(pKey, cKey, sizeof(BYTE)*count);

	// 새로운 키상태를 획득한다.
	::GetKeyboardState(cKey);

	// 클릭 상태를 체크한다.
	_check();
}
bool InputProcessor::AnyKey() const
{
	for (int i = 0; i < count; i++)
	{
		if ((cKey[i] & 0x80) == 0x80)
		{
			return true;
		}
	}

	return false;
}
bool InputProcessor::OnClick(const int& vkey)
{
	return clickbits[vkey];
}
void InputProcessor::Processed(const int& vkey)
{
   clickbits[vkey] = false;
   cKey[vkey] = 0x00;
}
bool InputProcessor::IsPressed(const int& vkey)
{
	return ((cKey[vkey] & 0x80) == 0x80);
}
bool InputProcessor::operator [] (const int& vkey)
{
	return IsPressed(vkey);
}
Point InputProcessor::pos() const
{
	Point tmp;
	::GetCursorPos(&tmp);

	return tmp;
}
void InputProcessor::_check()
{
	pressedlist.clear();

	for (int i = 0; i < count; i++)
	{
		if ((cKey[i] & 0x80) == 0x80)
		{
			pressedlist.push_back(i);

			UINT mvk = ::MapVirtualKey(i, MAPVK_VK_TO_VSC_EX);

			if (mvk != i && mvk != 0)
				pressedlist.push_back(mvk);
		}

		if ((cKey[i] & 0x80) != 0x80 && (pKey[i] & 0x80) == 0x80)
		{
			clickbits[i] = true;
		}
		else
		{
			clickbits[i] = false;
		}

	}
}
InputProcessor::KeyList& InputProcessor::current()
{
	return pressedlist;
}
