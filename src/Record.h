#ifndef RECORD_H_
#define RECORD_H_

#include "Backtrace.h"
#include <stdint.h>

enum struct RecordType : unsigned
{
	Alloc, Free
};

struct Record
{
	static const unsigned MaxDepth = Backtrace::MaxDepth;

	RecordType		Type;
	uintptr_t		Ptr;
	size_t			Size;
	_Unwind_Ptr		BacktraceData[MaxDepth];

	Record()
	{ }
	Record(RecordType type, void *ptr, size_t size = 0): Type(type), Ptr(reinterpret_cast<uintptr_t>(ptr)), Size(size), BacktraceData()
	{ Backtrace::Get(BacktraceData, MaxDepth); }
};

#endif

