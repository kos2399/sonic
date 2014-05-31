#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include "BaseType.h"

enum BBoxType
{
   BBOX_CIRCLE,
   BBOX_SQUARE,
};

class BoundaryBox
{
public :
   BoundaryBox(const int& t);
   virtual ~BoundaryBox();

   virtual void Draw(HDC) = 0;

   virtual bool IsCollide(BoundaryBox* other) = 0;
   virtual void SetPosition(const Point& pt) = 0;

   int type() const;
protected :
   int _type;
};

class CircleBox : public BoundaryBox
{
public :
   CircleBox();
   virtual ~CircleBox();

   virtual void Draw(HDC);
   virtual bool IsCollide(BoundaryBox* other);

   void SetPosition(const Point& pt);
   void SetRadius(const LONG& r);
private :
   Point center;
   LONG radius;
};

class SquareBox : public BoundaryBox
{
public :
   SquareBox();
   virtual ~SquareBox();

   virtual void Draw(HDC);
   virtual bool IsCollide(BoundaryBox* other);

   void SetPosition(const Point& pt);
   void SetRect(const Rect& rc);
   Rect getBox() const;
private :
   Rect rcBox;
};
