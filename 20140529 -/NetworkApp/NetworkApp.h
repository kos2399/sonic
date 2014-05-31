#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "../GameDev/GameDev.h"
#include <process.h>
#include <sstream>
#include <string>
#include <list>
#include "myserver.h"

class NetworkApp : public MainWindow<NetworkApp>
{
	typedef NetworkApp Me;
	typedef MainWindow<NetworkApp> Base;

	typedef std::list<Control*> ControlListType;
public :
	NetworkApp()
		: Server(NULL)
	{
		Log << "NetworkApp Start." << std::endl;
	}
	~NetworkApp()
	{
		Log << "NetworkApp End." << std::endl;
	}
	void Input(DWORD tick)
	{
		ControlListType::iterator it;
		for (it = ControlList.begin(); it != ControlList.end(); it++)
		{
			(*it)->Input(tick);
		}
	}
	void Update(DWORD tick)
	{
		ControlListType::iterator it;
		for (it = ControlList.begin(); it != ControlList.end(); it++)
		{
			(*it)->Update(tick);
		}
	}
	void Draw(DWORD)
	{
		int lineheight = 40;
		Rect box(rcClient.left, rcClient.top, rcClient.right, rcClient.top + lineheight);

		std::wostringstream oss;
		oss << _T("Client Count : ") << clientList.size();
		::DrawText(backbuffer, oss.str().c_str(), -1, &box, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		box = box >> Size(0, lineheight);

		std::list<clientinfo*>::iterator it;
		for (it = clientList.end(); it != clientList.end(); it++)
		{
			std::wostringstream oss;

			oss << _T("ID : " ) << (*it)->getId();

			::DrawText(backbuffer, oss.str().c_str(), -1, &box, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			box = box >> Size(0, lineheight);
		}

		ControlListType::iterator cit;
		for (cit = ControlList.begin(); cit != ControlList.end(); cit++)
		{
			(*cit)->Draw(backbuffer);
		}


		backbuffer.Draw();
	}

	void Enter()
	{
		Control* pControl;
		
		for (int i = 0; i < 3; i++)
		{
			pControl = new Button;
			pControl->Attach(hMainWnd);

			std::wostringstream oss;
			oss << i << _T(" Button");

			dynamic_cast<Button*>(pControl)->SetButtonText(oss.str().c_str());
			dynamic_cast<Button*>(pControl)->SetDrawRect(Rect(0,0,100,50)>>Size(50,50)>>Size(101*i, 0));
			dynamic_cast<Button*>(pControl)->SetStateColor(Color("aaaaaa"), Color("FF5555"), Color("007FFF"));
			ControlList.push_back(pControl);
		}

		Log << "Enter in." << std::endl;

		::InitializeCriticalSection(&cs);

		Log << "thread before." << std::endl;

		unsigned id;
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &Me::proxy, this, 0, &id);

		Log << "thread after." << std::endl;
	}
	void Leave()
	{
		ControlListType::iterator it;
		for (it = ControlList.begin(); it != ControlList.end();)
		{
			delete (*it);
			it = ControlList.erase(it);
		}

		::DeleteCriticalSection(&cs);
	}

	void listen()
	{
		Log << "listen Start." << std::endl;

		Server = new myserver("8887");
		Server->seterrbuf(logger::getReference().rdbuf());
		Log << "Server allocated : " << Server << std::endl;

		Server->Run();


		while (Server->listen() == 0)
		{
			Log << "while in." << std::endl;

			clientinfo* pInfo = NULL;
			if (pInfo = Server->accept())
			{
				Log << "accepted." << std::endl;
				Server->receive();
				Server->send();
			}

			::EnterCriticalSection(&cs);

			clientList = Server->getClientList();

			::LeaveCriticalSection(&cs);

			Log << "while out." << std::endl;
		}

		Log << "listen End." << std::endl;
	}

	static unsigned int __stdcall proxy(void* arg)
	{
		Log << "proxy Start." << std::endl;

		Me* pThis = reinterpret_cast<Me*>(arg);

		pThis->listen();

		Log << "proxy End." << std::endl;

		return 0;
	}

protected :
	void InitEventMap()
	{
		Base::InitEventMap();

		SetEventHandler(WM_BUTTON_CLICK, &Me::OnButtonClick);
	}
	LRESULT OnButtonClick(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		std::wostringstream oss;
		oss << wParam << _T(" Button Click");

		::MessageBox(hWnd, oss.str().c_str(), _T("Confirm"), MB_OK);

		return 0;
	}
private :
	myserver* Server;
	std::list<clientinfo*> clientList;

	CRITICAL_SECTION cs;

	ControlListType ControlList;
};
