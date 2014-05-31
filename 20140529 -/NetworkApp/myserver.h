#pragma once

#include "server.h"
#include "packet.h"

class myserver : public server
{
public :
	myserver(const char* _szPort = NULL)
		: server(_szPort)
		, recvbuffersize(512)
	{
	}
	~myserver()
	{
		close();
	}

	void Run()
	{
		initialize();
		createsocket();
		bind();
	}

	int send()
	{
		int iResult = 0;

		std::list<clientinfo*> cilist = getClientList();
		std::list<clientinfo*>::iterator it;
		for (it = cilist.begin(); it != cilist.end(); it++)
		{
			iResult = ::send((*it)->getSocket(), (const char*)&for_send, *(int*)(unsigned short*)for_send.cbSize, 0);
			if (iResult == SOCKET_ERROR)
			{
				std::cerr << "send failed : " << ::WSAGetLastError() << std::endl;

				::closesocket((*it)->getSocket());
				release();

				return 1;
			}
		}

		std::clog << "Sent : " << iResult << std::endl;


		return 0;
	}

	int receive()
	{
		if (getSocket() == INVALID_SOCKET)
		{
			std::cerr << "Could not use socket for recv." << std::endl;
			return 2;
		}

		int iResult = 0;
		int recvlen = recvbuffersize;

		std::list<clientinfo*> cilist = getClientList();
		std::list<clientinfo*>::iterator it;
		for (it = cilist.begin(); it != cilist.end(); it++)
		{
			if ((*it)->getSocket() == INVALID_SOCKET)
				continue;

			iResult = ::recv((*it)->getSocket(), (char*)&for_recv, recvlen, 0);
			if (iResult > 0)
			{
				std::clog << "Received : " << iResult << std::endl;
			}
			else if (iResult == 0)
			{
				std::clog << "Connection closing..." << std::endl;
			}
			else
			{
				std::clog << "recv failed : " << ::WSAGetLastError() << std::endl;
				::closesocket((*it)->getSocket());
				
				release();
			}
		}

		return 0;
	}

private :

	packet for_send;
	packet for_recv;

	const int recvbuffersize;
};