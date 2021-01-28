#include "Manager.h"
/*
MemoryAllocator::MemoryAllocator():destroyed(false),initialize(false)
{
	
}

MemoryAllocator::~MemoryAllocator()
{
	destroy();
}*/
/*
void MemoryAllocator::init()
{
	initialize = true;
	fsa16 = new FSA(16);
	fsa32 = new FSA(32);
	fsa64 = new FSA(64);
	fsa128 = new FSA(128);
	fsa256 = new FSA(256);
	fsa512 = new FSA(512);

	ca = new CA();
	
}

void MemoryAllocator::destroy()
{
	assert(initialize);
	
	destroyed = true;
	delete 	fsa16 ;
	delete 	fsa32 ;
	delete 	fsa64 ;
	delete 	fsa128 ;
	delete 	fsa256 ;
	delete 	fsa512 ;
	delete ca;
}*/
/*
void* MemoryAllocator::alloc(size_t size)
{
	assert(initialize);
	
	if (size <= 16) { return fsa16->alloc(size); }
	else if(size <= 32) { return fsa32->alloc(size); }
	else if(size <= 64) { return fsa64->alloc(size); }
	else if (size <= 128) { return fsa128->alloc(size); }
	else if (size <= 256) { return fsa256->alloc(size); }
	else if (size <= 512) { return fsa512->alloc(size); }
	else if (size <= 10 *1024)
	{
		return	ca->alloc(size);
	}
	else return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	
	return nullptr;
}*/
/*
void MemoryAllocator::free(void* p)
{
	assert(initialize);
	
	if (this->fsa16->belongToThat(p)) fsa16->free(p);
	else if (this->fsa32->belongToThat(p)) fsa32->free(p);
	else if (this->fsa64->belongToThat(p)) fsa64->free(p);
	else if (this->fsa128->belongToThat(p)) fsa128->free(p);
	else if (this->fsa256->belongToThat(p)) fsa256->free(p);
	else if (this->fsa512->belongToThat(p)) fsa512->free(p);
	else if (ca->belongToThat(p)) ca->free(p);
	else VirtualFree(p, 0, MEM_RELEASE);

}*/
/*
void MemoryAllocator::dumpStat() const
{
	assert(initialize);
}*/
/*
void MemoryAllocator::dumpBlocks() const
{
	assert(initialize);
}*/
