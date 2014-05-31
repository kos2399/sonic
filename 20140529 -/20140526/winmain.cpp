#include "GameApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst,
					 HINSTANCE,
					 LPTSTR, INT)
{
	GameApp app;

	app.Setup(hInst);

	return app.Run();
}