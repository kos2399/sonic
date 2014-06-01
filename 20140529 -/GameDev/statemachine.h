#pragma once

#include "../GameDev/GameDev.h"
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <map>

class statemachine;
// Idle과 walk의 부모
class state
{
public :
   state();
   virtual ~state();

   virtual void Enter() = 0;
   virtual void Input(DWORD) = 0;
   virtual void Update(DWORD) = 0;
   virtual void Draw(HDC) = 0;
   virtual void Leave() = 0;

   virtual void SetPosition(const Point& ) = 0;

   void SetMachine(statemachine* _pMachine);

private :

protected :
	// statemachine 키 
	statemachine* pMachine;
};

class statemachine
{
   typedef std::wstring key_type;
   //문자열을 넣을 수 있는 keytpe, state 포인터
   typedef std::map<key_type, state*> StateDepotType;
public :
   statemachine();
   virtual ~statemachine();

   //state 의 변환 ._T"walk"값이 들어감.
   state* transition(const key_type& key);

   bool AddEntry(const key_type& key, state* s);
   bool RemoveEntry(const key_type& key);

   state* current();

protected :
   state* _current;

   StateDepotType StateDepot;
};

