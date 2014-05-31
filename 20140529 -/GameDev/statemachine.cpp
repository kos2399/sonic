#include "statemachine.h"

state::state()
{}
state::~state()
{}
void state::SetMachine(statemachine* _pMachine)
{
	pMachine = _pMachine;
}

statemachine::statemachine()
   : _current(NULL)
{}
statemachine::~statemachine()
{}

state* statemachine::transition(const key_type& key)
{
   StateDepotType::iterator it = StateDepot.find(key);
   if (it == StateDepot.end())
   {
      Log << _T("Wrong Try State Transition.") << std::endl;

      return _current;
   }

   state* next = it->second;

   next->Enter();

   if (_current)
	_current->Leave();

   state* prev = _current;

   _current = next;

   return prev;
}
bool statemachine::AddEntry(const key_type& key, state* s)
{
   StateDepotType::iterator it = StateDepot.find(key);
   if (it == StateDepot.end())
   {
	   s->SetMachine(this);
      StateDepot.insert(std::make_pair(key, s));

      return true;
   }

   Log << _T("Wrong Try AddEntry.") << std::endl;

   return false;
}
bool statemachine::RemoveEntry(const key_type& key)
{
   StateDepotType::iterator it = StateDepot.find(key);
   if (it == StateDepot.end())
   {
      Log << _T("Wrong Try RemoveEntry.") << std::endl;
      return false;
   }

   StateDepot.erase(it);

   return true;
}
state* statemachine::current()
{
	return _current;
}