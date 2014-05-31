#include "NetworkApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst,
					 HINSTANCE,
					 LPTSTR, INT)
{
	NetworkApp app;

	app.Setup(hInst);

	return app.Run();
}