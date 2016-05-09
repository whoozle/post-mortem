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

std::atomic<void *> Monitor::_terminationAddress(NULL);
std::atomic<bool> Monitor::_globalBypass(false);


#define PRINTF(...) do { \
	char buf[64]; \
	size_t r = snprintf(buf, sizeof(buf), __VA_ARGS__); \
	write(STDOUT_FILENO, buf, r);\
} while(false)

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
	if (_globalBypass.load())
		return;

	if (_bypass)
	{
		//PRINTF("bypassed malloc %p, %u\n", p, size);
		//this hack allow library to unload. backtrace deallocates some of its internal structures and freezes while getting backtrace of this 'free'
#ifdef __mips
		//fixme: make it really atomic and configurable
		if (size == 20 && !_terminationAddress.load())
			_terminationAddress.store(p);
#endif
	}

	if (!p || _bypass)
		return;

	int fd = GetFD();
	Record record(RecordType::Alloc, p, size);
	write(fd, static_cast<void *>(&record), sizeof(record));
}

void Monitor::Free(void *p)
{
	//PRINTF("free %p\n", p);

	if (_globalBypass.load())
		return;

	if (_terminationAddress.load() == p)
	{
		_globalBypass = true;
		return;
	}

	if (!p || _bypass)
		return;

	int fd = GetFD();
	Record record(RecordType::Free, p);
	write(fd, static_cast<void *>(&record), sizeof(record));
}
