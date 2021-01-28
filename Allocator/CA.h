#pragma once
//#include "memoryapi.h"
#include "FSA.h"
#include <cassert>
#include <iostream>


#define  MINIMUM 20	


#define CA_SIZE 10  * 1024 * 1024


struct memoryBlock
{
	memoryBlock* next;
	memoryBlock* prev;
	int bytes;

	memoryBlock(memoryBlock* next, memoryBlock* prev, int size): next(next),prev(prev),bytes(size)
	{	}
	
};

//CoaleseAllocator pages
struct CApage
{
	void* buffer;
	memoryBlock* headBlock;
	

	CApage():nextPage(nullptr)
	{
/*
		buffer = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		memoryBlock tmp(nullptr, nullptr, size);

		*((memoryBlock*)(buffer)) = tmp;
		headBlock = ((memoryBlock*)(buffer));*/
	}
	
	~CApage() {
	}

	void initPage()
	{
		nextPage = nullptr;
		
		auto tmp = (char*)this;
		tmp += sizeof(CApage);
		buffer = (void*)tmp;

		memoryBlock block(nullptr, nullptr, CA_SIZE);
		*((memoryBlock*)(buffer)) = block;
		headBlock = ((memoryBlock*)(buffer)); 
		
	}

	void free(void* p)
	{
		char* pointer = (char*)p;
		pointer -= sizeof(int);
		int size = *((int*)pointer);

		/*
		memoryBlock* old = headBlock;
		headBlock = (memoryBlock*)pointer;
		headBlock->next = old; old->prev = headBlock;
		headBlock->bytes = sizeof(int) + size;
	*/
		
		memoryBlock* tmp = headBlock;

		while (tmp)
		{
			char* start = (char*)tmp;
			char* end = (char*)tmp; end += tmp->bytes; end--;

			char* pointerStart = (char*)pointer; pointerStart--;
			char* pointerEnd = (char*)pointer; pointerEnd += sizeof(int); pointerEnd += size;

			if (end == pointerStart)
			{

				tmp->bytes += size; tmp->bytes += sizeof(int);
				
				return;
			}
			else if (start == pointerEnd)
			{
				if (tmp->next) tmp->next->prev = (memoryBlock*)pointer;
				if (tmp->prev) tmp->prev->next = (memoryBlock*)pointer;
				(*((memoryBlock*)pointer)).bytes = tmp->bytes + size + sizeof(int);
				(*((memoryBlock*)pointer)).next = tmp->next; (*((memoryBlock*)pointer)).prev = tmp->prev;
				headBlock = (memoryBlock*)pointer;
				return;
			}
			tmp = tmp->next;
		}
	}

	

	
	char* alloc(int size)
	{
		if (headBlock->bytes < size + MINIMUM)		//не можем выделить в этом блоке
		{
			memoryBlock* tmp = headBlock->next;		// ищем в следующем 
			while (tmp)
			{
				if (tmp->bytes >= size + MINIMUM)
				{
					auto oldBlock = *tmp;			//разыменованная копия для того чтобы копировать потом данные 

					char* pointer = (char*)tmp;		//записываем сайз и двигаемся
					*((int*)pointer) = size;
					pointer += sizeof(int);

					char* newBlock = pointer; newBlock += size;		//двигаемся к новому блоку
					oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size; oldBlock.prev->next = (memoryBlock*)newBlock;
					if (oldBlock.next) oldBlock.next->prev = (memoryBlock*)newBlock; 
					*((memoryBlock*)newBlock) = oldBlock;

					/*
					if (oldBlock.bytes == size + 4) oldBlock.prev->next = oldBlock.next;		//блок полностью выпадает
					else if(oldBlock.bytes > size +4)
					{
						char* newBlock = pointer; newBlock += size;
						oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size;
						*((memoryBlock*)newBlock) = oldBlock;
					}*/

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
			pointer += sizeof(int);			//указатель на место куда вставляем структуру

			char* newBlock = pointer; newBlock += size;		//двигаемся к новому блоку
			oldBlock.bytes -= sizeof(int); oldBlock.bytes -= size;

			if (oldBlock.next) oldBlock.next->prev = (memoryBlock*)newBlock;
			
			*((memoryBlock*)newBlock) = oldBlock;
			headBlock = (memoryBlock*)newBlock;
			headBlock->next = oldBlock.next;
	
			return pointer;
		}


		return nullptr;
	}

	
	
	CApage* nextPage;

};


class CA
{
public:
	CA():pages(nullptr) {
	}

	
	~CA() {
		/*CApage* p;
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
		}*/
	}


	void init()
	{
		pages = AllocPage();
		pages->initPage();
	}

	void destroy()
	{
		CApage* p;
		while (pages)
		{
			p = pages;
			while (p->nextPage)
			{
				p = p->nextPage;
			}
			if (p == pages) { freePage(pages); return; }
			freePage(p);
			CApage* pn = pages;
			while (pn->nextPage != p) pn = pn->nextPage;
			pn->nextPage = nullptr;

		}
	}

	
	CApage* AllocPage()
	{
		const int data = CA_SIZE;
		const int dataAndFooter = data + sizeof(CApage);

		//todo: проверить ошибку выделения памяти
		auto pointer = VirtualAlloc(NULL, dataAndFooter, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		return (CApage*)pointer;
	}

	void freePage(CApage* pointer)
	{
		VirtualFree(pointer, 0, MEM_RELEASE);
	}


	
	void* alloc(size_t size) {

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

			CApage* tmp = AllocPage();	//делаем новую страницу
			tmp->initPage();
			tmp->nextPage = pages;
			pages = tmp;
			toReturn = (void*)(pages->alloc(size));
			return toReturn;

		}
		else return toReturn;
	}

	
	void free(void* p) {

		CApage* tmp = pages;

		while (tmp)
		{
			auto a = (((char*)p - (char*)tmp->buffer) );

			if (a >= 0 && a < CA_SIZE) { tmp->free(p); return; }
			else { tmp = tmp->nextPage; }


		}

#ifdef _DEBUG
		std::cout << "ERROR: can not free thar pointer " << p << "\n";
		assert(0); //
#endif



	/*
	auto a = (((char*)p - (char*)pages->buffer) / sizeof(char));
	if(a>0 && a< 8192)pages->free(p);
	else { std::cout << "ERROR: can not free that pointer " << p; }*/

	}
	

	CApage* pages;
public:
	bool belongToThat(void* p);
	

};

inline bool CA::belongToThat(void* p)
{
	
	CApage* tmp = pages;

	while (tmp)
	{
		auto a = (((char*)p - (char*)tmp->buffer) );

		if (a >= 0 && a < CA_SIZE) {  return true; }
		else { tmp = tmp->nextPage; }

	}
	return false;
}

