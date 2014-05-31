#pragma once

template<typename ReturnType, typename Cls, typename FunctionPointer>
struct Callable
{
	typedef FunctionPointer action_type;

	Callable(FunctionPointer fp):act(fp){}

	template<typename Derived,
		typename Arg1,
		typename Arg2,
		typename Arg3,
		typename Arg4>
	ReturnType operator ()(Derived* obj, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4)
	{
		return (get_pointer(obj)->*act)(a1,a2,a3,a4);
	}
	template<typename Derived>
	Cls* get_pointer(Derived* obj)
	{
		return dynamic_cast<Cls*>(obj);
	}

	FunctionPointer act;
};