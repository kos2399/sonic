#include "BaseType.h"

///////////////////////////////////////////////////////////////
// Point
///////////////////////////////////////////////////////////////
Point::Point(const LONG& _x/*=0*/, const LONG& _y/*=0*/)
{
	x = _x;
	y = _y;
}
Point Point::ToScreen(HWND hWnd)
{
	Point tmp(*this);

	::ClientToScreen(hWnd, &tmp);

	return tmp;
}
Point Point::ToClient(HWND hWnd)
{
	Point tmp(*this);

	::ScreenToClient(hWnd, &tmp);

	return tmp;
}
Point Point::operator << (const Size& cs)
{
	return Point(x - cs.cx, y - cs.cy);
}
Point Point::operator >> (const Size& cs)
{
	return Point(x + cs.cx, y + cs.cy);
}
Point Point::operator - (const Point& o)
{
	return Point(x - o.x, y - o.y);
}
float Point::operator ^ (const Point& o)
{
	Point diff = *this - o;
	return float(sqrt(float(diff.x*diff.x + diff.y*diff.y)));
}

///////////////////////////////////////////////////////////////
// Size
///////////////////////////////////////////////////////////////
Size::Size(const LONG& _cx/*=0*/, const LONG& _cy/*=0*/)
{
	cx = _cx;
	cy = _cy;
}
Size Size::operator - ()
{
	return Size(-cx, -cy);
}
Size operator * (const LONG& v, const Size& obj)
{
	return Size(obj.cx*v, obj.cy*v);
}
Size operator * (const Size& obj, const LONG& v)
{
	return Size(obj.cx*v, obj.cy*v);
}
Size operator * (const float& v, const Size& obj)
{
	return Size(LONG(obj.cx*v), LONG(obj.cy*v));
}
Size operator * (const Size& obj, const float& v)
{
	return Size(LONG(obj.cx*v), LONG(obj.cy*v));
}

///////////////////////////////////////////////////////////////
// Rect
///////////////////////////////////////////////////////////////
Rect::Rect(const LONG& l/*=0*/, const LONG& t/*=0*/, const LONG& r/*=0*/, const LONG& b/*=0*/)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
Rect::Rect(const Point& lt, const Point& rb)
{
	left = lt.x;
	top = lt.y;
	right = rb.x;
	bottom = rb.y;
}
Rect::Rect(const Point& ct, const Size& cs)
{
	left = ct.x - cs.cx/2;
	top = ct.y - cs.cy/2;
	right = left + cs.cx;
	bottom = top + cs.cy;
}
Rect::Rect(const Point& lt, const LONG& w, const LONG& h)
{
   left = lt.x;
   top = lt.y;
   right = left + w;
   bottom = top + h;
}
Point Rect::lefttop() const
{
	return Point(left, top);
}
Point Rect::rightbottom() const
{
	return Point(right, bottom);
}
LONG Rect::width() const
{
	return (right - left);
}
LONG Rect::height() const
{
	return (bottom - top);
}
Point Rect::center() const
{
	return Point((left+right)/2, (top+bottom)/2);
}
float Rect::radius() const
{
   return sqrt(float(width()*width() + height()*height()))/2;
}
Rect Rect::ToScreen(HWND hWnd)
{
	Point lt = lefttop();
	Point rb = rightbottom();

	return Rect(lt.ToScreen(hWnd), rb.ToScreen(hWnd));
}
Rect Rect::ToClient(HWND hWnd)
{
	Point lt = lefttop();
	Point rb = rightbottom();

	return Rect(lt.ToClient(hWnd), rb.ToClient(hWnd));
}
Rect Rect::Inflate(const Size& diff) const
{
	return Rect(left - diff.cx, top - diff.cy, right + diff.cx, bottom + diff.cy);
}
Rect Rect::Offset(const Size& diff) const
{
	return Rect(left + diff.cx, top + diff.cy, right + diff.cx, bottom + diff.cy);
}
Rect Rect::operator << (const Size& diff) const
{
	return Rect(left - diff.cx, top - diff.cy, right - diff.cx, bottom - diff.cy);
}
Rect Rect::operator >> (const Size& diff) const
{
	return Rect(left + diff.cx, top + diff.cy, right + diff.cx, bottom + diff.cy);
}
Rect Rect::operator + (const Size& diff) const
{
	return Rect(left - diff.cx, top - diff.cy, right + diff.cx, bottom + diff.cy);
}
Rect Rect::operator - (const Size& diff) const
{
	return Rect(left + diff.cx, top + diff.cy, right - diff.cx, bottom - diff.cy);
}

///////////////////////////////////////////////////////////////
// Color
///////////////////////////////////////////////////////////////

Color::Color(const COLORREF& _clr/* = RGB(255,255,255)*/)
: clr(_clr), diff(10)
{
}
Color::Color(const BYTE& r, const BYTE& g, const BYTE& b)
: clr(RGB(r,g,b)), diff(10)
{
}
Color::Color(const char* szColor)
: diff(10)
{
	clr = s2d(szColor);
}
// 16진수를 뜻하는 문자를 숫자로 변환하여 반환.
int Color::c2d(const char& c)
{
	if (c >= '0' && c <= '9')
		return int(c-'0');
	else if (c >= 'a' && c <= 'f')
		return int(c-'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return int(c-'A' + 10);

	return 0;
}
COLORREF Color::s2d(const char* s)
{
	BYTE r, g, b;
	r = c2d(s[0])*16 + c2d(s[1]);
	g = c2d(s[2])*16 + c2d(s[3]);
	b = c2d(s[4])*16 + c2d(s[5]);

	return RGB(r,g,b);
}
Color::operator COLORREF ()
{
	return clr;
}
Color& Color::operator ++ ()
{
	BYTE r = min(255, GetRValue(clr)+diff);
	BYTE g = min(255, GetGValue(clr)+diff);
	BYTE b = min(255, GetBValue(clr)+diff);

	clr = RGB(r,g,b);

	return *this;
}
Color Color::operator ++ (int)
{
	Color tmp(*this);

	++(*this);

	return tmp;
}
Color& Color::operator -- ()
{
	BYTE r = max(0, GetRValue(clr)-diff);
	BYTE g = max(0, GetGValue(clr)-diff);
	BYTE b = max(0, GetBValue(clr)-diff);

	clr = RGB(r,g,b);

	return *this;
}
Color Color::operator -- (int)
{
	Color tmp(*this);

	--(*this);

	return tmp;
}
Color Color::operator ! ()
{
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);

	return Color(255-r, 255-g, 255-b);
}
Color Color::operator - (const Color& o) const
{
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);

	BYTE ro = GetRValue(o.clr);
	BYTE go = GetGValue(o.clr);
	BYTE bo = GetBValue(o.clr);

	return Color(max(0, r-ro),max(0, g-go),max(0, b-bo));
}
Color Color::operator - (const BYTE& byte) const
{
	return Color(max(0,GetRValue(clr)-byte), max(0,GetGValue(clr)-byte), max(0,GetBValue(clr)-byte));
}
Color Color::operator + (const Color& o) const
{
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);

	BYTE ro = GetRValue(o.clr);
	BYTE go = GetGValue(o.clr);
	BYTE bo = GetBValue(o.clr);

	return Color(min(255, r+ro),min(255, g+go),min(255, b+bo));
}
Color Color::operator + (const BYTE& byte) const
{
	return Color(min(255,GetRValue(clr)+byte), min(255,GetGValue(clr)+byte), min(255,GetBValue(clr)+byte));
}
bool Color::operator == (const Color& o) const
{
	return (clr == o.clr);
}
bool Color::operator != (const Color& o) const
{
	return (clr != o.clr);
}
Color Color::operator / (const int i) const
{
	return Color(GetRValue(clr)/i, GetGValue(clr)/i, GetBValue(clr)/i);
}
Color Color::operator >> (const Color& o) const
{
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);

	BYTE ro = GetRValue(o.clr);
	BYTE go = GetGValue(o.clr);
	BYTE bo = GetBValue(o.clr);

	if (r > ro)
	{
		if (r-ro < 10)
			r = ro;
		else
			r -= (r-ro)/10;
	}
	else if (r < ro)
	{
		if (ro-r < 10)
			r = ro;
		else
			r += (ro-r)/10;
	}

	if (g > go)
	{
		if (g-go < 10)
			g = go;
		else
			g -= (g-go)/10;
	}
	else if (g < go)
	{
		if (go-g < 10)
			g = go;
		else
			g += (go-g)/10;
	}

	if (b > bo)
	{
		if (b-bo < 10)
			b = bo;
		else
			b -= (b-bo)/10;
	}
	else if (b < bo)
	{
		if (bo-b < 10)
			b = bo;
		else
			b += (bo-b)/10;
	}

	return Color(r,g,b);
}