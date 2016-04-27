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
	_Unwind_Ptr		BacktraceData[MaxDepth];

	Record(RecordType type): Type(type), BacktraceData()
	{ Backtrace::Get(BacktraceData, MaxDepth); }
};

#endif

