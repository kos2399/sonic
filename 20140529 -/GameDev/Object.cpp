#include "Object.h"

//
unsigned long Object::gen_id = 0;

Object::Object()
   : _id(gen_id++)
   , hOwner(NULL)
   , BBox(NULL)
{}
Object::~Object()
{}

void Object::Attach(HWND hWnd)
{
   hOwner = hWnd;
}
//_id는 gen_id 보다 +1 크다.
unsigned long Object::id() const
{
   return _id;
}
HWND Object::GetOwner() const
{
   return hOwner;
}
BoundaryBox* Object::getBoundaryBox() const
{
   return BBox;
}