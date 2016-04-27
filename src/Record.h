#ifndef RECORD_H_
#define RECORD_H_

#include "Backtrace.h"

enum struct RecordType : unsigned
{
	Alloc, Free
};

struct Record
{
	static const unsigned MaxDepth = Backtrace::MaxDepth;

	RecordType		Type;
	void *			Ptr;
	size_t			Size;
	_Unwind_Ptr		BacktraceData[MaxDepth];

	Record()
	{ }
	Record(RecordType type, void *ptr, size_t size = 0): Type(type), Ptr(ptr), Size(size), BacktraceData()
	{ Backtrace::Get(BacktraceData, MaxDepth); }
};

#endif

