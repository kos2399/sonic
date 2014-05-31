#include "DoubleBuffer.h"

DoubleBuffer::DoubleBuffer()
: hOwner(NULL), hMainDC(NULL), hMemDC(NULL)
, hMemBitmap(NULL), hOldMemBitmap(NULL)
{
}
DoubleBuffer::~DoubleBuffer()
{
	Detach();
}
void DoubleBuffer::Attach(HWND hWnd)
{
	if (hWnd == NULL) return;

	Detach();

	hOwner = hWnd;
	::GetClientRect(hOwner, &rcClient);
	hMainDC = ::GetDC(hOwner);
	hMemDC = ::CreateCompatibleDC(hMainDC);
	hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rcClient.width(), rcClient.height());
	hOldMemBitmap = ::Select(hMemDC, hMemBitmap);
}
void DoubleBuffer::Detach()
{
	if (hMemBitmap)
	{
		::Select(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);

		hMemBitmap = NULL;
	}
	if (hMemDC)
	{
		::DeleteDC(hMemDC);

		hMemDC = NULL;
	}
	if (hMainDC)
	{
		::ReleaseDC(hOwner, hMainDC);

		hMainDC = NULL;
	}

	hOwner = NULL;
}
void DoubleBuffer::Draw()
{
	::BitBlt(hMainDC, 0, 0, rcClient.width(), rcClient.height(),
		hMemDC, 0, 0, SRCCOPY);
}
DoubleBuffer::Me& DoubleBuffer::operator << (const COLORREF& color)
{
	::SetDCBrushColor(hMemDC, color);
	::FillRect(hMemDC, &rcClient, (HBRUSH)::GetStockObject(DC_BRUSH));

	return *this;
}
DoubleBuffer::operator HDC ()
{
	return hMemDC;
}

