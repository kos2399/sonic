#pragma once

#include "client.h"
#include "packet.h"

class myclient : public client
{
public :
	myclient(const char* _szServer = NULL, const char* _szPort = NULL)
		: client(_szServer, _szPort)
		, recvbuffersize(512)
	{
	}
	~myclient()
	{
	}

	int send(void)
	{
		int iResult = 0;

		for (int i = 0; i < 2; i++)
		{
			for_send.cbSize[i] = (unsigned char)((unsigned short)sizeof(packet) >> i*8);
		}
		for_send.btType = PT_DUMMY;
		for_send.data = 0;

		char buffer[100] = {"0123456789"};

		iResult = ::send(getSocket(), buffer, 10, 0);
		//iResult = ::send(getSocket(), (const char*)&for_send, *(int*)(unsigned short*)for_send.cbSize, 0);
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "send failed : " << ::WSAGetLastError() << std::endl;

			::closesocket(getSocket());
			release();

			return 1;
		}

		std::clog << "Sent : " << iResult << std::endl;


		return 0;
	}

	int receive(void)
	{
		if (getSocket() == INVALID_SOCKET)
		{
			std::cerr << "Could not use socket for recv." << std::endl;
			return 2;
		}

		int iResult = 0;
		int recvlen = recvbuffersize;

		iResult = ::recv(getSocket(), (char*)&for_recv, recvlen, 0);
		if (iResult > 0)
		{
			std::clog << "Received : " << iResult << std::endl;
		}
		else if (iResult == 0)
		{
			std::clog << "Connection closed." << std::endl;
		}
		else
		{
			std::clog << "recv failed : " << ::WSAGetLastError() << std::endl;
		}

		return 0;
	}

private :
	packet for_send;
	packet for_recv;

	const int recvbuffersize;
};