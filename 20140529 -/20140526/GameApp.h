#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"
#include "Character.h"

class GameApp : public MainWindow<GameApp>
{
	typedef GameApp Me;
	typedef MainWindow<GameApp> Base;
public :
	GameApp();
	~GameApp();

	void Input(DWORD);
	void Update(DWORD);
	void Draw(DWORD);

protected :
	void Enter();
	void Leave();

	void InitEventMap();
	LRESULT OnSize(HWND,UINT,WPARAM,LPARAM);

private :
	Character* pUser; 
	Point ptMouse;

	float theta;
};
