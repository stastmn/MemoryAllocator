/*#include "CA.h"
#include <cassert>
#include <iostream>

CApage::CApage(int size):nextPage(nullptr)
{
	
	buffer = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	memoryBlock tmp(nullptr, nullptr, size);
	
	*((memoryBlock*)(buffer)) = tmp;
	headBlock = ((memoryBlock*)(buffer));
}

CApage::~CApage()
{
	VirtualFree(buffer, 0, MEM_RELEASE);
}

void CApage::free(void* p)
{
	char* pointer = (char*)p;
	pointer -= sizeof(int);
	int size = *((int*)pointer);

	/*memoryBlock* old = headBlock;
	headBlock = (memoryBlock*)pointer;
	headBlock->next = old; old->prev = headBlock;
	headBlock->bytes = sizeof(int) + size;
*//*
	memoryBlock* tmp = headBlock->next;
	
	while(tmp)
	{
		char* start = (char*)tmp; 
		char* end = (char*)tmp; end += tmp->bytes; end--;

		char* pointerStart = (char*)pointer; pointerStart--;
		char* pointerEnd = (char*)pointer; pointerEnd += sizeof(int); pointerEnd += size;
		
		if (end == pointerStart)
		{
			
			tmp->bytes += size; tmp->bytes += sizeof(int);
			
		}
		else if (start == pointerEnd)
		{
			if (tmp->next) tmp->next->prev = (memoryBlock*)pointer;
			if (tmp->prev) tmp->prev->next = (memoryBlock*)pointer;
			if (tmp == headBlock) headBlock = (memoryBlock*)pointer;
			tmp->bytes += size; tmp->bytes += sizeof(int);
			
		}


		tmp = tmp->next;
	}
	
	
}

char* CApage::alloc(int size)		//plus secret variable for saving size and oth
{
	if(headBlock->bytes < size + MINIMUM)		//не можем выделить
	{
		memoryBlock* tmp = headBlock->next;
		while(tmp)
		{
			if(tmp->bytes>= size + MINIMUM)
			{
				auto oldBlock = *tmp;			//разыменованная копия для того чтобы копировать потом данные 
				
				char* pointer = (char*)tmp;		//записываем сайз и двигаемся
				*((int*)pointer) = size;
				pointer += sizeof(int);

				char* newBlock = pointer; newBlock += size;		//двигаемся к новому блоку
				oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size; oldBlock.prev->next = (memoryBlock*)newBlock;
				if(oldBlock.next) oldBlock.next->prev = (memoryBlock*)newBlock;
				*((memoryBlock*)newBlock) = oldBlock;

				/*
				if (oldBlock.bytes == size + 4) oldBlock.prev->next = oldBlock.next;		//блок полностью выпадает
				else if(oldBlock.bytes > size +4)
				{
					char* newBlock = pointer; newBlock += size;
					oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size;
					*((memoryBlock*)newBlock) = oldBlock;
				}*//*
			
				return pointer;
				
				
			}
			tmp = tmp->next;
		}
		return nullptr;
	}
	else
	{
		auto oldBlock = *headBlock;			//разыменованная копия для того чтобы копировать потом данные 

		char* pointer = (char*)headBlock;		//записываем сайз и двигаемся
		*((int*)pointer) = size;
		pointer += sizeof(int);

		char* newBlock = pointer; newBlock += size;		//двигаемся к новому блоку
		oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size;
		
		if(oldBlock.next) oldBlock.next->prev = (memoryBlock*)newBlock;
		*((memoryBlock*)newBlock) = oldBlock;
		headBlock = (memoryBlock*)newBlock;
		headBlock->next = oldBlock.next;
		return pointer;
	}

	
	return nullptr;
}

CA::CA()
{
	pages = new CApage();
}

CA::~CA()
{
	CApage* p;
	while (pages)
	{
		p = pages;
		while (p->nextPage)
		{
			p = p->nextPage;
		}
		if (p == pages) { delete pages; return; }
		delete p;
		p = nullptr;
	}
}

void* CA::alloc(size_t size)
{
/*	auto a = pages->alloc(size);
	if (a == nullptr) { std::cout << "ERROR: can not allocate that size!"; assert(0); }
	else return (void*)a;*//*

	void* toReturn;

	toReturn = (void*)(pages->alloc(size));		//сразу получили память

	if (toReturn == nullptr)					//не получили
	{
		CApage* p = pages->nextPage;
		while (p)			//пробуем посмотреть в других страницах
		{
			toReturn = (void*)(p->alloc(size));
			if (toReturn) return toReturn;

			p = p->nextPage;
		}

		CApage* tmp = new CApage();		//делаем новую страницу
		tmp->nextPage = pages;
		toReturn = (void*)(pages->alloc(size));
		return toReturn;

	}
	else return toReturn;
}

void CA::free(void* p)
{

	CApage* tmp = pages;

	while (tmp)
	{
		auto a = (((char*)p - (char*)tmp->buffer) /8192);

		if (a >= 0 && a < 8192) { tmp->free(p); return; }
		else { tmp = tmp->nextPage; }

		
	}

#ifdef _DEBUG
	std::cout << "ERROR: can not free thar pointer " << p << "\n";
	assert(0); //
#endif


	
	/*
	auto a = (((char*)p - (char*)pages->buffer) / sizeof(char));
	if(a>0 && a< 8192)pages->free(p);
	else { std::cout << "ERROR: can not free that pointer " << p; }*//*
	
}*/
