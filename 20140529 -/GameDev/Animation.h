#pragma once

#include <vector>
#include "Image.h"
#include "Manager.hpp"

class Animation
{
	typedef std::vector<Image*> depot;
	typedef depot::iterator depotiter;
public :
	Animation();
	~Animation();

	Animation* clone();

	void reset();
	void SetImage(depot& imagelist);
	void AddImage(Image* pImage);
	void SetLoop(bool _loop = true);
	void SetDelay(DWORD _delay = 500);
	void SetPosition(const Point& pt);
	void SetRect(const Rect& rc);
	Rect getRect(void) const;

	void Update(DWORD tick);
	void Draw(HDC hdc, const Rect& rc);
	void Draw(HDC hdc);

	void SetTransparent(COLORREF clr);

	bool IsPlaying() const;

	// 알파블랜딩으로 그리기 위한 설정.
	void SetAlpha(BYTE alpha);

private :
	depot shotlist;
	int index;
	DWORD delay;
	DWORD update_dt;
	bool bLoop;
	bool bDoNotDraw;
	Point ptDraw;
	Rect rcImage;
	Rect rcDest;
};


class AnimationManager : public Manager<Animation>, public singleton<AnimationManager>
{
};

#define AniDepot AnimationManager::getReference()