#ifndef PM_MONITOR_H
#define PM_MONITOR_H

#include "Record.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

class Monitor
{
	int fd;

public:
	Monitor(): fd(open("post-mortem.log", O_WRONLY))
	{ if (fd == -1) abort(); }

	void Alloc(void *p)
	{
		Record record(RecordType::Alloc);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}

	void Free(void *p)
	{
		Record record(RecordType::Free);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}
};

#endif

