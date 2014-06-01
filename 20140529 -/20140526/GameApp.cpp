#include "GameApp.h"

GameApp::GameApp()
: theta(0.f)
, pUser(NULL)
{
	SetWindowStyle(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX);
	SetWindowSize(1330, 500);
}
GameApp::~GameApp()
{
}

void GameApp::Input(DWORD tick)
{
	ptMouse = InputDevice.pos().ToClient(hMainWnd);

	if (InputDevice[VK_RETURN])
	{
		enterPress = true;
	}

	//stage2
		pUser->Input(tick);
}
void GameApp::Update(DWORD tick)
{
	
	AniDepot["cursor"]->SetPosition(ptMouse);
	AniDepot["cursor"]->Update(tick);

	AniDepot["sonic"]->Update(tick);
	AniDepot["press"]->Update(tick);

	if(enterPress)
	{
		if (update_dt > update_delay)
		{
			gameStart = true;
			update_dt %= update_delay;
		}
		update_dt += tick;
	}
			//stage2
	if (pUser)
		pUser->Update(tick);
}
void GameApp::Draw(DWORD)
{

	if (!gameStart){
		//1. 메인
		ImgDepot["title"]->Draw(backbuffer);
		ImgDepot["one"]->Draw(backbuffer);
		if (!enterPress)
		{
			ImgDepot["press"]->Draw(backbuffer);
		}

		AniDepot["cursor"]->Draw(backbuffer);
		AniDepot["sonic"]->Draw(backbuffer);
		if (enterPress){
			AniDepot["press"]->Draw(backbuffer);
		}
	}
	if(gameStart)
	{
		ImgDepot["map"]->Draw(backbuffer);
		pUser->Draw(backbuffer);
		pUser->SetBBoxSize(pUser->GetPosition(), Size(20, 20));
		/*block->Draw(backbuffer);*/
	}
		
	backbuffer.Draw();
}

void GameApp::Enter()
{
	pt.x = 500 / 2;
	pt.y = (400 / 2) - 50;
	Rect rc(0, 0, 400, 250);

	ImgLoader(_T("Resource//test.image"));
	  
	 ImgDepot["title"]->SetRect(rcClient);
	 ImgDepot["press"]->Move(pt<<Size(0,-150));
	 ImgDepot["one"]->SetRect(rc);
	 ImgDepot["one"]->Move(pt);

	 Rect rcMap(0,0,502,402);
	 Point SonicPosition(50,100);
	
	 ////stage 2
	 ImgDepot["map"]->SetRect(rcMap);
	     //케릭터
	 pUser = new Character;
	 pUser->SetPosition(SonicPosition);
	 pUser->SetShowBox(true);
	 pUser->SetBBoxSize(SonicPosition, Size(20, 20));
	     //블럭
	 

	 AniLoader(_T("Resource//animation.animation"));
	 AniDepot["sonic"]->SetPosition(pt);
	 AniDepot["press"]->SetPosition(pt << Size(0, -150));


}
void GameApp::Leave()
{
	
}
void GameApp::InitEventMap()
{
	Base::InitEventMap();

	SetEventHandler(WM_SIZE, &Me::OnSize);
}
LRESULT GameApp::OnSize(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
	Rect rc(0,0,500,400);

	::AdjustWindowRect(&rc, dwStyle, FALSE);

	::SetWindowPos(hWnd, NULL, 0, 0,
		rc.width(), rc.height(), 
		SWP_NOZORDER | SWP_NOMOVE);

	ChangeClientRect();

	return 0;
}
