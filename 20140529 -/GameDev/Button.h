#pragma once

#include "Control.h"
#include "InputDevice.h"
#include "Utility.hpp"

enum WM_USER_ID
{
	WM_BUTTON_CLICK = WM_USER + 1,
};

class Button : public Control
{
protected :
	enum
	{
		MOUSE_NORMAL,
		MOUSE_HOVER,
		MOUSE_CLICK,
		MAX_MOUSE_STATE,
	};
public :
	Button();
	virtual ~Button();

	virtual void Input(DWORD tick);
	virtual void Update(DWORD tick);
	virtual void Draw(HDC hdc);

	void SetButtonText(LPCTSTR text);
	void SetCaption(LPCTSTR caption);
	void SetDrawRect(const Rect& rc);
	void SetStateColor(const Color& normal, const Color& hover, const Color& click);

protected :
	Rect rcDraw;

	LPTSTR szText;
	LPTSTR szCaption;

	int prevstate;
	int state;
	Color clrState[MAX_MOUSE_STATE];
	Color clrCurrent;
	bool clicked;

	DWORD update_dt;
	DWORD update_delay;
	bool bUpdate;

	Point ptMouse;
};
