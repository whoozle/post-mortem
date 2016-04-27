#ifndef PM_MONITOR_H
#define PM_MONITOR_H

#include "Record.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

class Monitor
{
	static int _fd;

public:
	static int GetFD()
	{
		if (_fd < 0)
		{
			_fd = open("post-mortem.log", O_WRONLY | O_CREAT | O_TRUNC, 0664);
			if (_fd == -1)
			{
				static const char *error = "open: cannot open post-mortem.log\n";
				write(STDOUT_FILENO, error, strlen(error));
				abort();
			}
		}
		return _fd;
	}

	static void Alloc(void *p)
	{
		int fd = GetFD();
		Record record(RecordType::Alloc);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}

	static void Free(void *p)
	{
		int fd = GetFD();
		Record record(RecordType::Free);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}
};

int Monitor::_fd = -1;

#endif

