#pragma once

#pragma once

#include <fstream>
#include <iomanip>
#include <windows.h>
#include "Utility.hpp"

class logger : public std::fstream, public singleton<logger>
{
	friend class singleton<logger>;
private :
	logger();
	~logger();

public :
	logger& current();
};

#define Log logger::getReference().current()