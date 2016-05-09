#ifndef MALLOC_H
#define MALLOC_H

#include <dlfcn.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#if defined(__GNU_LIBRARY__) && !defined(__UCLIBC__)
#	define LIBC_FUNCTIONS
#endif

#ifdef LIBC_FUNCTIONS
extern "C"
{
	void *__libc_malloc(size_t size);
	void __libc_free(void *ptr);
	void *__libc_calloc(size_t nmemb, size_t size);
	void *__libc_realloc(void *ptr, size_t size);
}
#endif

#define GET_SYMBOL(name) _##name = reinterpret_cast<decltype(_##name)>(dlsym(RTLD_NEXT, #name))

namespace Malloc
{
	void * (*_malloc)(size_t size) = 0;
	void (*_free)(void *ptr) = 0;
	void * (*_calloc)(size_t nmemb, size_t size) = 0;
	void * (*_realloc)(void *ptr, size_t size) = 0;

	int (*_posix_memalign)(void **memptr, size_t alignment, size_t size) = 0;
	void *(*_aligned_alloc)(size_t alignment, size_t size) = 0;
	void *(*_valloc)(size_t size) = 0;
	void *(*_memalign)(size_t alignment, size_t size) = 0;
	void *(*_pvalloc)(size_t size) = 0;

	void Init()
	{
		if (_malloc)
			return;

#ifdef LIBC_FUNCTIONS
		_malloc = &__libc_malloc;
		_free = &__libc_free;
		_realloc = &__libc_realloc;
		_calloc = &__libc_calloc;
#else
		GET_SYMBOL(malloc);
		GET_SYMBOL(free);
		GET_SYMBOL(realloc);
		GET_SYMBOL(calloc);
#endif
		if (!_malloc || !_free || !_calloc || !_realloc)
		{
			static const char *error = "dlsym(RTLD_NEXT) failed\n";
			write(STDOUT_FILENO, error, strlen(error));
			abort();
		}

		GET_SYMBOL(posix_memalign);
		GET_SYMBOL(aligned_alloc);
		GET_SYMBOL(valloc);
		GET_SYMBOL(memalign);
		GET_SYMBOL(pvalloc);
	}

	static void * Alloc(size_t size)
	{
		Init();
		return _malloc(size);
	}

	static void Free(void *ptr)
	{
		Init();
		_free(ptr);
	}

	static void * Calloc(size_t nmemb, size_t size)
	{ Init(); return _calloc(nmemb, size); }

	static void * Realloc(void *ptr, size_t size)
	{
		Init();
		return _realloc(ptr, size);
	}

	static int PosixMemAlign(void **ptr, size_t alignment, size_t size)
	{
		Init();
		return _posix_memalign(ptr, alignment, size);
	}

	static void *AlignedAlloc(size_t alignment, size_t size)
	{
		Init();
		return _aligned_alloc(alignment, size);
	}

	static void *Valloc(size_t size)
	{
		Init();
		return _valloc(size);
	}

	static void *MemAlign(size_t alignment, size_t size)
	{
		Init();
		return _memalign(alignment, size);
	}

	static void *PValloc(size_t size)
	{
		Init();
		return _pvalloc(size);
	}
}

#endif
