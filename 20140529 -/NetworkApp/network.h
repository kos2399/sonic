#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")

class network
{
public :
	network()
		: _Socket(INVALID_SOCKET)
	{
	}
	virtual ~network()
	{
	}

	void seterrbuf(std::basic_streambuf<char>* errbuf)
	{
		std::cerr.rdbuf(errbuf);
		std::clog.rdbuf(errbuf);
		std::cout.rdbuf(errbuf);
	}

	// initialize winsock
	int initialize(void)
	{
		int iResult;

		iResult = ::WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0)
		{
			std::cerr << "WSAStartup failed : " << iResult << std::endl;

			// error code
			//WSASYSNOTREADY;
			//WSAVERNOTSUPPORTED;
			//WSAEINPROGRESS;
			//WSAEPROCLIM;
			//WSAEFAULT;

			return iResult;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			std::cerr << "Could not find a usable version of Winsock.dll" << std::endl;

			::WSACleanup();

			return 1;
		}

		std::clog << "Highest Version : " << (int)LOBYTE(wsaData.wHighVersion) 
			<< "." << (int)HIBYTE(wsaData.wHighVersion) << std::endl;
		std::clog << "Max Socket : " << wsaData.iMaxSockets << std::endl;
		std::clog << "Max Msg Size : " << wsaData.iMaxUdpDg << std::endl;
		std::clog << "Description : " << wsaData.szDescription << std::endl;
		std::clog << "System Status : " << wsaData.szSystemStatus << std::endl;

		std::clog << "initialize success" << std::endl;
		// successful result.
		return 0;
	}
	int release(void)
	{
		::closesocket(_Socket);
		::WSACleanup();

		std::clog << "release success" << std::endl;
		return 0;
	}

protected :
	inline SOCKET& getSocket(void) 
	{
		return _Socket;
	}
	inline void setSocket(const SOCKET& Socket = INVALID_SOCKET)
	{
		_Socket = Socket;
	}

private :
	WSADATA wsaData;
	SOCKET _Socket;

};