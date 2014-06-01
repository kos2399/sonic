#pragma once

#include "../GameDev/GameDev.h"

#include "Character.h"


//평소 상태
class Idle : public state
{
public :
	Idle();
	virtual ~Idle();

	 void Enter();
	 void Input(DWORD);
	 void Update(DWORD);
	 void Draw(HDC);
	 void Leave();

	void SetPosition(const Point& pt);
	void SetRadius(const LONG& r);
	Point GetPosition();

private :
	DWORD update_dt;
	DWORD update_delay;
	//원이 꿀렁 꿀렁 움직이는 처리
	bool bFat;

	Point pos;
	LONG width;
	LONG height;
};
// 걷는 상태
class Walk : public state
{
	// 걷는 상태는 앞으로 가고 뒤로가고
	enum { none, forward, backward };
public :
	Walk();
	virtual ~Walk();

	virtual void Enter();
	virtual void Input(DWORD);
	virtual void Update(DWORD);
	virtual void Draw(HDC);
	virtual void Leave();

	void SetPosition(const Point& pt);
	void SetRadius(const LONG& r);
	Point GetPosition();

private :
	DWORD update_dt;
	DWORD update_delay;

	bool bFat;

	int nMove;

	Point pos;

	LONG width;
	LONG height;
};