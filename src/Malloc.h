#ifndef MALLOC_H
#define MALLOC_H

#include <dlfcn.h>
#include <malloc.h>

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

namespace Malloc
{
	void * (*_malloc)(size_t size) = 0;
	void (*_free)(void *ptr) = 0;
	void * (*_calloc)(size_t nmemb, size_t size) = 0;
	void * (*_realloc)(void *ptr, size_t size) = 0;

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
		_malloc	= reinterpret_cast<decltype(_malloc)>(dlsym(RTLD_NEXT, "malloc"));
		_free	= reinterpret_cast<decltype(_free)>(dlsym(RTLD_NEXT, "free"));
		_realloc= reinterpret_cast<decltype(_realloc)>(dlsym(RTLD_NEXT, "realloc"));
		_calloc	= reinterpret_cast<decltype(_calloc)>(dlsym(RTLD_NEXT, "calloc"));
#endif
		if (!_malloc || !_free || !_calloc || !_realloc)
		{
			static const char *error = "dlsym(RTLD_NEXT) failed\n";
			write(STDOUT_FILENO, error, strlen(error));
			abort();
		}
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
}

#endif
