#include "Character.h"

Character::Character()
: bShowBox(true)
{
	BBox = new CircleBox();

	init();
}
Character::~Character()
{
	if (BBox)
		delete BBox;
}

void Character::Input(DWORD tick)
{
	if (_current)
		_current->Input(tick);
}
void Character::Update(DWORD tick)
{
	if (_current)
		_current->Update(tick);
}
void Character::Draw(HDC hdc)
{
	if (bShowBox)
	{
		if (BBox)
		{
			BBox->Draw(hdc);
		}
	}
	if (_current)
		_current->Draw(hdc);
}
//SetBBoxsize.
void Character::SetBBoxSize(const Point& pt, const Size& cs)
{
	//BoundaryBox *BBox
	if (BBox)
	{
		if (BBox->type() == BBOX_CIRCLE)
		{
			dynamic_cast<CircleBox*>(BBox)->SetPosition(pt);
			dynamic_cast<CircleBox*>(BBox)->SetRadius(cs.cx);
		}
		else if (BBox->type() == BBOX_SQUARE)
		{
			dynamic_cast<SquareBox*>(BBox)->SetRect(Rect(pt, cs));
		}
	}
}
void Character::SetShowBox(bool bShow/*=true*/)
{
	bShowBox = bShow;
}
void Character::SetPosition(const Point& pt)
{
	pos = pt;

	StateDepotType::iterator it;
	for (it = StateDepot.begin();
		it != StateDepot.end(); it++)
	{
		it->second->SetPosition(pos);
	}
}

void Character::init()
{
	static Idle idle;
	static Walk walk;

	idle.SetRadius(20);
	walk.SetRadius(20);

	AddEntry(_T("idle"), &idle);
	AddEntry(_T("walk"), &walk);

	transition(_T("idle"));
}
