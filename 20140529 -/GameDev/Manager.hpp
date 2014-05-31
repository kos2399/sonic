#pragma once

#include <string>
#include <map>

template<typename T, typename KeyType = std::string>
class Manager
{
protected :
	typedef T element;
	typedef element* pointer;
	typedef element& reference;
	typedef std::map<KeyType,pointer> Container;
	typedef typename Container::iterator Iter;
public :
	Manager()
	{
	}
	virtual ~Manager()
	{
		_release();
	}

public :
	// 등록시 이미 등록되어 있는지 없는지 확인하여
	// 이미 등록되어 있다면 false
	// 없어서 새로이 등록하였다면 true
	// .. 를 반환하도록 수정.
	bool push(const KeyType& id, pointer pEntity)
	{
		if (!pEntity) return false;

		Iter it = depot.find(id);
		if (it != depot.end())
			return false;

		depot.insert(std::make_pair(id, pEntity));

		return true;
	}
	pointer get(const KeyType& id)
	{
	  Iter it = depot.find(id);
	  if (it == depot.end())
		 return NULL;

	  return it->second;
	}
	pointer clone(const KeyType& id)
	{
		return get(id)->clone();
	}
	void pop(const KeyType& id)
	{
		Iter it = depot.find(id);
		if (it != depot.end())
			depot.erase(it);
	}
	pointer operator [] (const KeyType& id)
	{
	  Iter it = depot.find(id);
	  if (it == depot.end())
		 return NULL;

	  return it->second;
	}

private :
	void _release()
	{
		for (Iter it = depot.begin();
			  it != depot.end();)
		{
			delete it->second;
			it = depot.erase(it);
		}
	}
protected :
	Container depot;
};
