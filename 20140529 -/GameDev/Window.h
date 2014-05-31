#pragma once

#include "Control.h"
#include "InputDevice.h"
#include "Utility.hpp"

class Window : public Control
{
protected :
   enum
   {
      AREA_NORMAL,
      AREA_TITLE,
      AREA_TITLE_TEXT,
      AREA_BORDER,
      MAX_AREA,
   };
public :
   Window();
   virtual ~Window();

   virtual void Input(DWORD);
   virtual void Update(DWORD);
   virtual void Draw(HDC);

   void SetDrawRect(const Rect& rc);
   void SetWindowTitle(LPCTSTR title, const Color& clr = Color("000000"));
   void SetWindowColor(const Color& title, const Color& border, const Color& area);

protected :
   Rect rcDraw;
   Rect rcTitle;
   Rect rcArea;

   LPTSTR szTitle;

   Color clrArea[MAX_AREA];

   bool bPrevPressed;
   bool bPressed;

   bool bGrabWindow;

   const LONG bordersize;
   const LONG fontHeight;

   Point ptMouse;
   Point ptPrevMouse;
};
