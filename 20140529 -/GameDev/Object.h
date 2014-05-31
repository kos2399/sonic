#pragma once

#include <Windows.h>
#include <tchar.h>
#include "BoundaryBox.h"

class Object
{
public :
   Object();
   virtual ~Object();

   void Attach(HWND);

   virtual void Input(DWORD) = 0;
   virtual void Update(DWORD) = 0;
   virtual void Draw(HDC) = 0;

   unsigned long id() const;
   HWND GetOwner() const;
   BoundaryBox* getBoundaryBox() const;
protected :
   HWND hOwner;
   BoundaryBox* BBox;
private :
   unsigned long _id;
   static unsigned long gen_id;
};
