#ifndef MALLOC_H
#define MALLOC_H

extern "C"
{
	void *__libc_malloc(size_t size);
	void __libc_free(void *ptr);
	void *__libc_calloc(size_t nmemb, size_t size);
	void *__libc_realloc(void *ptr, size_t size);
}

#endif
