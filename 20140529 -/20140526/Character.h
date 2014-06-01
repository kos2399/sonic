#pragma once

#include "../GameDev/GameDev.h"
#include "Character_state.h"



class Character : public Object, public statemachine
{
public :
	Character();
	~Character();

	void Input(DWORD);
	void Update(DWORD);
	void Draw(HDC);


	void SetBBoxSize(const Point& pt, const Size& cs);
	void SetShowBox(bool bShow = true);
	void SetPosition(const Point& pt);
	Point GetPosition();

private :
	void init();

private :
	//BoundaryBox
	BoundaryBox* BBox;
	bool bShowBox;
	Point pos;

};

