#ifndef PM_MONITOR_H
#define PM_MONITOR_H

class Monitor
{
	static int _fd;
	static __thread bool _bypass;

public:

	struct BypassEnable
	{
		BypassEnable()  { _bypass = true; }
		~BypassEnable() { _bypass = false; }
	};

	static int GetFD();

	static void Alloc(void *p, size_t size);
	static void Free(void *p);
};

#endif

