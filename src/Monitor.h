#ifndef PM_MONITOR_H
#define PM_MONITOR_H

#include <atomic>

class Monitor
{
	static int _fd;
	static thread_local int _bypass;
	static std::atomic<void *> _terminationAddress;
	static std::atomic<bool> _globalBypass;

public:

	struct BypassEnable
	{
		BypassEnable()  { ++_bypass; }
		~BypassEnable() { --_bypass; }
	};

	static void SetGlobalBypass(bool bypass)
	{ _globalBypass = bypass; }

	static int GetFD();

	static void Terminate();
	static void Alloc(void *p, size_t size);
	static void Free(void *p);
};

#endif

