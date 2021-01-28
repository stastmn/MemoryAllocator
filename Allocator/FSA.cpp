/*#include "FSA.h"
#include <cassert>
#include <iostream>

page::page(int numOfBlocks,int blockSize):headIndex(0),blockSize(blockSize),numOfBlocks(numOfBlocks),nextPage(nullptr)
{
	buffer = VirtualAlloc(NULL, blockSize* numOfBlocks, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	markupPage();
}

page::~page()
{
	VirtualFree(buffer, 0, MEM_RELEASE);
}

char* page::operator[](int index)
{
	return (char*)buffer + (index * blockSize);
}

void page::markupPage()
{
	for(int i =0;i< numOfBlocks-1;i++)
	{
		if(i == numOfBlocks-1) { *((int*)(this->operator[](i))) = -1; }
		*((int*)(this->operator[](i))) = i + 1;
	}
}

void page::free(void* p)
{
	*((int*)p) = headIndex;
	headIndex = ((char*)p - (char*)buffer) / blockSize;
}

char* page::alloc()
{
	if (headIndex == -1) return nullptr;
	else
	{
		char* temp = this->operator[](headIndex);
		headIndex = *((int*)temp);
		
		return temp;
	}
}

FSA::FSA(int blockSize, int numOfElements ):numOfBlocks(numOfElements),sizeOfBlocks(blockSize)
{
	pages = new page(numOfBlocks, sizeOfBlocks);
}

FSA::~FSA()
{
/*	page* p;
	while(pages)
	{
			p = pages;
		while(p->nextPage)
		{
			p = p->nextPage;
		}
		if (p == pages) { delete pages; return; }
		delete p;
		p = nullptr;
	}*//*
}

void* FSA::alloc(size_t size)
{
	void* toReturn;
	
	toReturn = (void*)(pages->alloc());		//сразу получили память
	
	if(toReturn == nullptr)					//не получили
	{
		page* p = pages->nextPage;
		while(p)			//пробуем посмотреть в других страницах
		{
			toReturn = (void*)(p->alloc());
			if (toReturn) return toReturn;
			
			p = p->nextPage;
		}
		
		page* tmp = new page(numOfBlocks, sizeOfBlocks);		//делаем новую страницу
		tmp->nextPage = pages;
		toReturn = (void*)(pages->alloc());
		return toReturn;
		
	}
	else return toReturn;
	
}

void FSA::free(void* p)
{
	page* tmp = pages;

	while(tmp)
	{
		auto a = (((char*)p - (char*)tmp->buffer) / sizeOfBlocks);
		
		if (a >= 0 && a < numOfBlocks) { tmp->free(p); return; }
		else { tmp = tmp->nextPage; }

	
	}

#ifdef _DEBUG
	std::cout << "ERROR: can not free thar pointer " << p << "\n";
	assert(0); //
#endif


}

bool FSA::belongToThat(void* p)
{
	page* tmp = pages;

	while (tmp)
	{
		auto a = (((char*)p - (char*)tmp->buffer) / sizeOfBlocks);

		if (a >= 0 && a < numOfBlocks) {  return true; }
		else { tmp = tmp->nextPage; }

	}

	return false;
}*/
