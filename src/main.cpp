#include <stddef.h>
#include "Monitor.h"
#include "Malloc.h"
#include <new>

__attribute__ ((visibility ("default")))
void *malloc(size_t size)
{
	void *p = __libc_malloc(size);
	Monitor::Alloc(p, size);
	return p;
}

__attribute__ ((visibility ("default")))
void free(void *ptr)
{
	Monitor::Free(ptr);
	__libc_free(ptr);
}

__attribute__ ((visibility ("default")))
void *calloc(size_t nmemb, size_t size)
{
	void *p = __libc_calloc(nmemb, size);
	Monitor::Alloc(p, nmemb * size);
	return p;
}

__attribute__ ((visibility ("default")))
void *realloc(void *ptr, size_t size)
{
	void *p = __libc_realloc(ptr, size);
	if (p) {
		Monitor::Free(ptr);
		Monitor::Alloc(p, size);
	}
	return p;
}


__attribute__ ((visibility ("default")))
void* operator new(size_t size)
{
	return malloc(size);
}

__attribute__ ((visibility ("default")))
void* operator new( size_t size, const std::nothrow_t& ) throw()
{
	return malloc(size);
}

__attribute__ ((visibility ("default")))
void* operator new[](size_t size)
{
	return malloc(size);
}

__attribute__ ((visibility ("default")))
void* operator new[](size_t size, const std::nothrow_t& ) throw()
{
	return malloc(size);
}

__attribute__ ((visibility ("default")))
void operator delete (void *p)
{
	return free(p);
}

__attribute__ ((visibility ("default")))
void operator delete (void *p, const std::nothrow_t& ) throw()
{
	return free(p);
}

__attribute__ ((visibility ("default")))
void operator delete[] (void *p)
{
	return free(p);
}

__attribute__ ((visibility ("default")))
void operator delete[] (void *p, const std::nothrow_t& ) throw()
{
	return free(p);
}
