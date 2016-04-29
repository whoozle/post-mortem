#ifndef PM_MONITOR_H
#define PM_MONITOR_H

#include "Record.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class Monitor
{
	static int _fd;
	static __thread bool _bypass;

	struct BypassEnable
	{
		BypassEnable()  { _bypass = true; }
		~BypassEnable() { _bypass = false; }
	};

public:
	static int GetFD()
	{
		if (_fd < 0)
		{
			char exenamebuf[BUFSIZ];
			readlink("/proc/self/exe", exenamebuf, BUFSIZ);
			char *exename = strrchr(exenamebuf, '/');
			if (exename == NULL)
				exename = exenamebuf;
			else
				++exename;
			char filename[BUFSIZ];
			snprintf(filename, sizeof(filename), "post-mortem.%s.%d.log", exename, getpid());
			_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
			if (_fd == -1)
			{
				static const char *error = "open: cannot open post-mortem.log\n";
				write(STDOUT_FILENO, error, strlen(error));
				abort();
			}
			unsigned version = 0;
			write(_fd, &version, sizeof(version));
		}
		return _fd;
	}

	static void Alloc(void *p, size_t size)
	{
		if (!p || _bypass)
			return;

		BypassEnable be;
		int fd = GetFD();
		Record record(RecordType::Alloc, p, size);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}

	static void Free(void *p)
	{
		if (!p || _bypass)
			return;

		BypassEnable be;
		int fd = GetFD();
		Record record(RecordType::Free, p);
		write(fd, static_cast<void *>(&record), sizeof(record));
	}
};

int Monitor::_fd = -1;
bool __thread Monitor::_bypass = false;

#endif

