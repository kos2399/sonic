#include "Button.h"

Button::Button()
: state(MOUSE_NORMAL), prevstate(MOUSE_NORMAL)
, clicked(false)
, update_dt(0), update_delay(20), bUpdate(false)
, szText(NULL), szCaption(NULL)
{}
Button::~Button()
{
	::SafeDelete(szText);
	::SafeDelete(szCaption);
}

void Button::Input(DWORD tick)
{
	ptMouse = InputDevice.pos();
	ptMouse = ptMouse.ToClient(hOwner);

	prevstate = state;

	if (::PtInRect(&rcDraw, ptMouse))
	{
		if (InputDevice[VK_LBUTTON])
		{
			state = MOUSE_CLICK;
			//clrCurrent = clrState[state];
		}
		else
		{
			state = MOUSE_HOVER;
			//clrCurrent = clrState[state];
		}

		if (InputDevice.OnClick(VK_LBUTTON))
			clicked = true;
	}
	else
	{
		state = MOUSE_NORMAL;
		//clrCurrent = clrState[state];
	}

	if (prevstate != state)
		bUpdate = true;
}
void Button::Update(DWORD tick)
{
	if (clicked)
	{
		::SendMessage(hOwner, WM_BUTTON_CLICK, id(), 0);

		clicked = false;
	}

	if (!bUpdate) return;

	if (update_dt > update_delay)
	{
		int count = update_dt / update_delay;

		for (int i = 0; i < count; i++)
		{
			clrCurrent = clrCurrent >> clrState[state];

			if (clrCurrent == clrState[state])
			{
				update_dt = 0;
				bUpdate = false;
				break;
			}
		}

		update_dt %= update_delay;
	}

	update_dt += tick;
}
void Button::Draw(HDC hdc)
{
	HBRUSH hBrush = ::CreateSolidBrush(clrCurrent);
	HBRUSH hOldBrush = ::Select(hdc, hBrush);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, clrCurrent);
	HPEN hOldPen = ::Select(hdc, hPen);

	::Rectangle(hdc, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);

	if (szText)
	{
		COLORREF clrOld = ::SetTextColor(hdc, clrCurrent-BYTE(70));

		int oldbkmode = ::GetBkMode(hdc);
		::SetBkMode(hdc, TRANSPARENT);
		::DrawText(hdc, szText, -1, &rcDraw, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		::SetBkMode(hdc, oldbkmode);

		::SetTextColor(hdc, clrOld);
	}

	if (szCaption)
	{
		// TODO
	}

	::Select(hdc, hOldPen);
	::DeleteObject(hPen);

	::Select(hdc, hOldBrush);
	::DeleteObject(hBrush);
}
void Button::SetButtonText(LPCTSTR text)
{
	::strAlloc(szText, text);
}
void Button::SetCaption(LPCTSTR caption)
{
	::strAlloc(szCaption, caption);
}
void Button::SetDrawRect(const Rect& rc)
{
	rcDraw = rc;
}
void Button::SetStateColor(const Color& normal, const Color& hover, const Color& click)
{
	clrState[MOUSE_NORMAL] = normal;
	clrState[MOUSE_HOVER] = hover;
	clrState[MOUSE_CLICK] = click;

	clrCurrent = clrState[MOUSE_NORMAL];
}
