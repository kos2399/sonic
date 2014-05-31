#pragma once

#include "GameDev.h"

class TestApp : public MainWindow<TestApp>
{
	typedef TestApp Me;
	typedef MainWindow<TestApp> Base;
public:
	TestApp(void);
	~TestApp(void);

public :
	virtual void Input(DWORD tick);
	virtual void Update(DWORD tick);
	virtual void Draw(DWORD tick);

protected :
	void InitEventMap()
	{
		Base::InitEventMap();

		SetEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		SetEventHandler(WM_PAINT, &Me::OnPaint);
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// todo
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::Ellipse(hdc, ptMouse.x - 20, ptMouse.y - 20, ptMouse.x + 20, ptMouse.y + 20);

		::EndPaint(hWnd, &ps);
		return 0;
	}

private :
	POINT ptMouse;
};
