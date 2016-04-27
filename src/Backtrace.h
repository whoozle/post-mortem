#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <unwind.h>

class Backtrace
{
public:
	static const unsigned MaxDepth = 24;
	_Unwind_Ptr Data[MaxDepth];

	Backtrace()
	{ Get(Data, MaxDepth); }

	static void Get(_Unwind_Ptr *data, size_t size);

private:
	void DoUnwind()
	{ }
};

#endif
