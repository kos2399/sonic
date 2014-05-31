#include "Utility.hpp"

void ErrorMessageBox(const DWORD& dwError)
{

	LPTSTR lpszError = NULL;
	DWORD dwFlag = FORMAT_MESSAGE_FROM_SYSTEM |
				   FORMAT_MESSAGE_ALLOCATE_BUFFER |
				   FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD dwLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

	DWORD dwRet = ::FormatMessage(dwFlag, NULL, dwError, dwLocale, (LPTSTR)&lpszError,
		0, NULL);

	if (dwRet && lpszError)
	{
		// 에러메세지를 메세지 박스를 이용해서 표시.
		::MessageBox(NULL, lpszError, _T("Error"), MB_OK | MB_ICONERROR);
	}
	else
	{
		DWORD err = ::GetLastError();

		int a = 1;
	}

	::LocalFree(lpszError);
}

