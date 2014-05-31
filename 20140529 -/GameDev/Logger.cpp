#include "Logger.h"

logger::logger()
{
	open("test.log", ios_base::out | ios_base::app, _SH_DENYRW);

	*this << "log in" << std::endl;
}
logger::~logger()
{
	*this << "log out" << std::endl;
	close();
}
logger& logger::current()
{
	::EnterCriticalSection(&cs);

	SYSTEMTIME st;
	::GetSystemTime(&st);

	*this << std::setw(4) << std::setfill('0') << st.wYear << "/";
	*this << std::setw(2) << std::setfill('0') << st.wMonth << "/";
	*this << std::setw(2) << std::setfill('0') << st.wDay << " ";
	*this << std::setw(2) << std::setfill('0') << st.wHour << ":";
	*this << std::setw(2) << std::setfill('0') << st.wMinute << ":";
	*this << std::setw(2) << std::setfill('0') << st.wSecond << ":";
	*this << std::setw(3) << std::setfill('0') << st.wMilliseconds << " ";

	::LeaveCriticalSection(&cs);

	return *this;
}
