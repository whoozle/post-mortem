#ifndef PM_MONITOR_H
#define PM_MONITOR_H

class Monitor
{
	static int _fd;
	static thread_local int _bypass;

public:

	struct BypassEnable
	{
		BypassEnable()  { ++_bypass; }
		~BypassEnable() { --_bypass; }
	};

	static int GetFD();

	static void Alloc(void *p, size_t size);
	static void Free(void *p);
};

#endif

