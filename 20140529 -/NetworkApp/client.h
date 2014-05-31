#pragma once

#include "network.h"

class client : public network
{
public :
	client(const char* _szServer = NULL, const char* _szPort = NULL)
		: ai(NULL)
	{
		setipport(_szServer, _szPort);
	}
	~client()
	{
	}

	int createsocket(const char* _szServer = NULL, const char* _szPort = NULL)
	{
		setipport(_szServer, _szPort);

		addrinfo hints;

		::memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = AF_INET; // for IPv4
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		int iResult;
		addrinfo *result = NULL;

		iResult = ::getaddrinfo(serveraddr, port, &hints, &result);
		if (iResult != 0)
		{
			std::cerr << "getaddrinfo failed : " << iResult << std::endl;

			release();

			return 1;
		}

		// result ...
		std::clog << std::endl;
		ai = result;

		setSocket(::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol));
		if (getSocket() == INVALID_SOCKET)
		{
			std::cerr << "Error at socket() : " << ::WSAGetLastError() << std::endl;
			::freeaddrinfo(ai);

			release();

			return 2;
		}

		std::clog << "createsocket success" << std::endl;
		// successful result..
		return 0;
	}

	int connect(void)
	{
		int iResult = 0;

		iResult = ::connect(getSocket(), ai->ai_addr, (int)ai->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			::closesocket(getSocket());
			setSocket();

			::freeaddrinfo(ai);

			if (getSocket() == INVALID_SOCKET)
			{
				std::cerr << "Unable to connect to server" << std::endl;

				release();
			}

			return 1;
		}

		std::clog << "connect success" << std::endl;
		// successful result..
		return 0;
	}

	int close(void)
	{
		int iResult = 0;
		iResult = ::shutdown(getSocket(), SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			std::cerr << "shutdown failed : " << ::WSAGetLastError() << std::endl;
			::closesocket(getSocket());
			release();

			return 1;
		}

		std::clog << "close success" << std::endl;
		return 0;
	}

protected :

private :
	void setipport(const char* _szServer, const char* _szPort)
	{
		if (_szServer)
		{
			::strcpy_s(serveraddr, 20, _szServer);
		}
		if (_szPort)
		{
			::strcpy_s(port, 10, _szPort);
		}
	}
private :

	char port[10];
	char serveraddr[20]; // IPv4

	addrinfo* ai;
};