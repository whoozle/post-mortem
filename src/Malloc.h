#ifndef MALLOC_H
#define MALLOC_H

#include <dlfcn.h>

namespace Malloc
{
	char _memory[256 * 1024];
	char *_memory_ptr = _memory;
	char *_memory_end = _memory + sizeof(_memory);

	bool _is_early(void *ptr_)
	{
		char *ptr = static_cast<char *>(ptr_);
		return ptr >= _memory && ptr < _memory_end;
	}

	void _early_alloc_error(const char *error)
	{
		write(STDOUT_FILENO, error, strlen(error));
		abort();
	}

	void * _early_malloc(size_t size)
	{
		char *next = _memory_ptr + size;
		if (next > _memory_end)
			_early_alloc_error("early allocator ran out of space\n");
		char *r = _memory_ptr;
		_memory_ptr = next;
		return r;
	}

	void _early_free(void *ptr)
	{ }

	void * _early_calloc(size_t nmemb, size_t size)
	{ void * p = _early_malloc(nmemb * size); memset(p, 0, size); return p; }

	void * _early_realloc(void *ptr, size_t size)
	{ _early_alloc_error("realloc\n"); return NULL; }

	void * (*_malloc)(size_t size) = _early_malloc;
	void (*_free)(void *ptr) = _early_free;
	void * (*_calloc)(size_t nmemb, size_t size) = _early_calloc;
	void * (*_realloc)(void *ptr, size_t size) = _early_realloc;

	void Init()
	{
		if (_malloc)
			return;

		_malloc	= reinterpret_cast<decltype(_malloc)>(dlsym(RTLD_NEXT, "malloc"));
		_free	= reinterpret_cast<decltype(_free)>(dlsym(RTLD_NEXT, "free"));
		_realloc= reinterpret_cast<decltype(_realloc)>(dlsym(RTLD_NEXT, "realloc"));
		_calloc	= reinterpret_cast<decltype(_calloc)>(dlsym(RTLD_NEXT, "calloc"));

		static const char *error2 = "INIT2\n";
		write(STDOUT_FILENO, error2, strlen(error2));

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
		if (_is_early(ptr))
			_early_free(ptr);
		else
			_free(ptr);
	}

	static void * Calloc(size_t nmemb, size_t size)
	{ Init(); return _calloc(nmemb, size); }

	static void * Realloc(void *ptr, size_t size)
	{
		Init();
		return (_is_early(ptr))? _early_realloc(ptr, size): _realloc(ptr, size);
	}
}

#endif
