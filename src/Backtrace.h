#ifndef BACKTRACE_H
#define BACKTRACE_H

#include <unwind.h>
#include <stddef.h>

class Backtrace
{
	struct State
	{
		_Unwind_Ptr *	Data;
		size_t			Size;
		_Unwind_Ptr		LastIP;
		_Unwind_Ptr		LastCFA;
	};

public:
	static const unsigned MaxDepth = 24;
	_Unwind_Ptr Data[MaxDepth];

	Backtrace()
	{ Get(Data, MaxDepth); }

	static void Get(_Unwind_Ptr *data, size_t size)
	{
		State state({ data, size, 0 , 0});
		_Unwind_Backtrace(&DoUnwind, &state);
	}

private:
	static _Unwind_Reason_Code DoUnwind(struct _Unwind_Context *ctx, void *state_)
	{
		State *state = static_cast<State *>(state_);
		_Unwind_Ptr ip = _Unwind_GetIP(ctx);
		_Unwind_Ptr cfa = _Unwind_GetCFA(ctx);
		_Unwind_Ptr lastIP = state->LastIP;
		_Unwind_Ptr lastCFA = state->LastIP;
		state->LastIP = ip;
		state->LastCFA = cfa;
		*state->Data++ = ip;
		--state->Size;
		return ip && ip != lastIP && cfa != lastCFA && state->Size? _URC_NO_REASON: _URC_END_OF_STACK;
	}
};

#endif
