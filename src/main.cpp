#include <stddef.h>
#include "Monitor.h"
#include "Malloc.h"
#include <new>


extern "C"
{

    __attribute__ ((visibility ("default")))
    void *malloc(size_t size)
    {
        void *p = Malloc::Alloc(size);
        Monitor::Alloc(p, size);
        return p;
    }

    __attribute__ ((visibility ("default")))
    void free(void *ptr)
    {
        Monitor::Free(ptr);
        Malloc::Free(ptr);
    }

    __attribute__ ((visibility ("default")))
    void *calloc(size_t nmemb, size_t size)
    {
        void *p = Malloc::Calloc(nmemb, size);
        Monitor::Alloc(p, nmemb * size);
        return p;
    }

    __attribute__ ((visibility ("default")))
    void *realloc(void *ptr, size_t size)
    {
        void *p = Malloc::Realloc(ptr, size);
        if (p)
        {
            Monitor::Free(ptr);
            Monitor::Alloc(p, size);
        }
        else if (size == 0)
        {
            Monitor::Free(ptr);
        }
        return p;
    }

    __attribute__ ((visibility ("default")))
    int posix_memalign(void **memptr, size_t alignment, size_t size)
    { return -1; }

    __attribute__ ((visibility ("default")))
    void *aligned_alloc(size_t alignment, size_t size)
    { return NULL; }

    __attribute__ ((visibility ("default")))
    void *valloc(size_t size)
    { return NULL; }

    __attribute__ ((visibility ("default")))
    void *memalign(size_t alignment, size_t size)
    { return NULL; }

    __attribute__ ((visibility ("default")))
    void *pvalloc(size_t size)
    { return NULL; }

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
