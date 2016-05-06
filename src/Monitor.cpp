#include "Record.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int Monitor::_fd = -1;
int __thread Monitor::_bypass = 0;

int Monitor::GetFD()
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

void Monitor::Alloc(void *p, size_t size)
{
	if (!p || _bypass)
		return;

	int fd = GetFD();
	Record record(RecordType::Alloc, p, size);
	write(fd, static_cast<void *>(&record), sizeof(record));
}

void Monitor::Free(void *p)
{
	if (!p || _bypass)
		return;

	int fd = GetFD();
	Record record(RecordType::Free, p);
	write(fd, static_cast<void *>(&record), sizeof(record));
}
