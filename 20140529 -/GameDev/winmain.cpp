#include <windows.h>
#include <tchar.h>
#include "TestApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	TestApp app;

	app.Setup(hInst);

   return app.Run();
		}