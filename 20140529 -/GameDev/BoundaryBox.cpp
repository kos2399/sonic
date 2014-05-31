#include "BoundaryBox.h"

BoundaryBox::BoundaryBox(const int& t)
   : _type(t)
{
}
BoundaryBox::~BoundaryBox()
{
}

int BoundaryBox::type() const
{
   return _type;
}

CircleBox::CircleBox()
   : BoundaryBox(BBOX_CIRCLE)
{
}
CircleBox::~CircleBox()
{
}

void CircleBox::Draw(HDC hdc)
{
   ::Ellipse(hdc, center.x-radius, center.y-radius,
      center.x+radius, center.y+radius);
}
bool CircleBox::IsCollide(BoundaryBox* other)
{
   if (other == NULL) return false;

   if (other->type() == BBOX_CIRCLE)
   {
      Point diff = center - dynamic_cast<CircleBox*>(other)->center;

      float dist = sqrt(float(diff.x*diff.x + diff.y*diff.y));

      std::wostringstream oss;
      oss << _T("c : ") << center.x << _T(", ") << center.y << _T("\n");
      oss << _T("d : ") << dynamic_cast<CircleBox*>(other)->center.x;
      oss << _T(", ") << dynamic_cast<CircleBox*>(other)->center.y << _T("\n");
      oss << _T("dist : ") << dist << _T("\n");
      ::OutputDebugString(oss.str().c_str());
      return (dist <= radius + dynamic_cast<CircleBox*>(other)->radius);
   }
   else if (other->type() == BBOX_SQUARE)
   {
      Rect rcBox = dynamic_cast<SquareBox*>(other)->getBox();
      Point diff = center - rcBox.center();

      if (abs(diff.x) > radius + rcBox.width()/2 || 
          abs(diff.y) > radius + rcBox.height()/2)
         return false;

      if (abs(diff.x) <= rcBox.width()/2 ||
         abs(diff.y) <= rcBox.height()/2)
         return true;

      float dist = sqrt(float(diff.x*diff.x + diff.y*diff.y));

      return (dist <= radius + rcBox.radius());
   }

   return false;
}
void CircleBox::SetPosition(const Point& pt)
{
   center = pt;
}
void CircleBox::SetRadius(const LONG& r)
{
   radius = r;
}

SquareBox::SquareBox()
   : BoundaryBox(BBOX_SQUARE)
{
}
SquareBox::~SquareBox()
{
}

void SquareBox::Draw(HDC hdc)
{
   ::Rectangle(hdc, rcBox.left, rcBox.top, rcBox.right, rcBox.bottom);
}
bool SquareBox::IsCollide(BoundaryBox* other)
{
   if (other == NULL) return false;

   if (other->type() == BBOX_SQUARE)
   {
      Rect oBox = dynamic_cast<SquareBox*>(other)->getBox();
      return !(rcBox.left > oBox.right ||
               rcBox.right < oBox.left ||
               rcBox.top > oBox.bottom ||
               rcBox.bottom < oBox.top);
   }
   else if (other->type() == BBOX_CIRCLE)
   {
      return other->IsCollide(this);
   }

   return false;
}

Rect SquareBox::getBox() const
{
   return rcBox;
}
void SquareBox::SetPosition(const Point& pt)
{
   rcBox = rcBox >> Size(pt.x, pt.y) << Size(rcBox.width()/2, rcBox.height()/2);
}
void SquareBox::SetRect(const Rect& rc)
{
   rcBox = rc;
}