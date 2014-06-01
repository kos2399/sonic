#pragma once

#include "../GameDev/GameDev.h"

class block :public Object
{

public:

	void setPoision();

	void Draw();

private:

	void init();

private:
	BoundaryBox* BBox;
	bool bShowBox;
	Point pos;

};