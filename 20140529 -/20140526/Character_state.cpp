﻿#include "Character_state.h"

//////////////////////////////////////////
// Idle
//////////////////////////////////////////
Idle::Idle()
: update_dt(0), update_delay(100)
, bFat(false)
{
}
Idle::~Idle()
{
}
void Idle::Enter()
{
}
// 키 입력 
void Idle::Input(DWORD)
{
	if (InputDevice[VK_LEFT])
	{
		pos.x -= 5;

		pMachine->transition(_T("walk"));
	}
	if (InputDevice[VK_RIGHT])
	{
		pos.x += 5;

		pMachine->transition(_T("walk"));
	}
	AniDepot["character"]->SetPosition(pos);
	
}
// 업데이트
void Idle::Update(DWORD tick)
{

	AniDepot["character"]->Update(tick);
	
}
void Idle::Draw(HDC hdc)
{
	Rect rc(0, 0,100,100);

	::DrawText(hdc, _T("idle"), -1, &rc, DT_TOP);

	AniDepot["character"]->Draw(hdc);
}
void Idle::Leave()
{
}

void Idle::SetPosition(const Point& pt)
{
	pos = pt;
	
	
}
void Idle::SetRadius(const LONG& r)
{
	width = r;
	height = r;
}
Point Idle::GetPosition()
{
	return pos;
}

//////////////////////////////////////////
// Walk
//////////////////////////////////////////
Walk::Walk()
: update_dt(0), update_delay(50)
, bFat(false), nMove(none)
{
}
Walk::~Walk()
{
}
void Walk::Enter()
{
}
void Walk::Input(DWORD)
{
	if (InputDevice.AnyKey())
	{
		bool bValidInput = false;

		if (InputDevice[VK_LEFT])
		{
			nMove = backward;

			bValidInput = true;
			pMachine->transition(_T("walk"));
		}
		if (InputDevice[VK_RIGHT])
		{
			nMove = forward;

			bValidInput = true;

			pMachine->transition(_T("walk"));
		}

		if (!bValidInput)
		{
			pMachine->transition(_T("idle"));
		}
		AniDepot["walk"]->SetPosition(pos);

	}
	else
	{
		pMachine->transition(_T("idle"));
	}
}
void Walk::Update(DWORD tick)
{
	if (update_dt > update_delay)
	{
		if (nMove == backward)
			pos.x -= 5;
		else if (nMove == forward)
			pos.x += 5;

		if (pMachine)
			dynamic_cast<Character*>(pMachine)->SetPosition(pos);





		update_dt -= update_delay;
	}
	update_dt += tick;
	AniDepot["walk"]->Update(tick);
}
void Walk::Draw(HDC hdc)
{
	Rect rc(0, 0, 100, 100);

	::DrawText(hdc, _T("walk"), -1, &rc, DT_TOP);

	std::wostringstream oss;
	oss << _T("pos : ") << pos.x << _T(", ") << pos.y << _T("\n");

	::OutputDebugString(oss.str().c_str());


	AniDepot["walk"]->Draw(hdc);
}
void Walk::Leave()
{
}
void Walk::SetPosition(const Point& pt)
{
	pos = pt;
}
void Walk::SetRadius(const LONG& r)
{
	width = height = r;
}
Point Walk::GetPosition()
{
	return pos;
}