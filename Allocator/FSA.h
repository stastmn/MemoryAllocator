#pragma once
#include "Windows.h"
#include "memoryapi.h"
#include <cassert>
#include <iostream>

#define PAGE_SIZE 4096

struct page
{
	void* buffer;
	page* nextPage;
	int headIndex;

	int blockSize;
	int numOfBlocks;

	void initPage( int blockSize)
	{
		assert(PAGE_SIZE > blockSize);
		
		numOfBlocks = PAGE_SIZE / blockSize;
		headIndex = 0;
		this->blockSize = blockSize;
		nextPage = nullptr;
		
		auto tmp = (char*)this;
		tmp += sizeof(page);
		buffer = (void*)tmp;

		markupPage();
	}
	

	

	char* operator[](int index)
	{
		return (char*)buffer + (index * blockSize);
	}
	
	void markupPage() {
		for (int i = 0; i < numOfBlocks; i++)
		{
			if (i == numOfBlocks - 1) { *((int*)(this->operator[](i))) = -1; continue;}
			*((int*)(this->operator[](i))) = i + 1;
		}
	}
	
	void free(void* p) {
		*((int*)p) = headIndex;
		headIndex = ((char*)p - (char*)buffer) / blockSize;
	}
	
	char* alloc()
	{
		if (headIndex == -1) return nullptr;
		else
		{
			char* temp = this->operator[](headIndex);
			headIndex = *((int*)temp);

			return temp;
		}
	}

private:
	


	page(int blockSize) :headIndex(0), blockSize(blockSize), nextPage(nullptr)
	{
		numOfBlocks = PAGE_SIZE / blockSize;
	}

	~page()
	{
	}
	
};




class FSA
{
public:
	FSA(int blockSize):numOfBlocks(PAGE_SIZE / blockSize),sizeOfBlocks(blockSize), pages(nullptr)
	{
	}

	void init()
	{
		pages = AllocPage();
		pages->initPage(sizeOfBlocks);
	}

	void destroy()
	{
		page* p;
		while (pages)
		{
			p = pages;
			while (p->nextPage)
			{
				p = p->nextPage;
			}
			if (p == pages) { freePage(pages); return; }
			freePage(p);
			page* pn = pages;
			while (pn->nextPage != p) pn = pn->nextPage;
			pn->nextPage = nullptr;
		}
	}
	
	//- Короче, Меченый, я тебе страницу выделил и в благородство играть не буду: аллоцируешь для меня пару объектов — и мы в расчете. 
	page* AllocPage()
	{
		const int data = sizeOfBlocks * numOfBlocks;
		const int dataAndFooter = data + sizeof(page);

		//todo: обработать ошибку выделения памяти
		auto pointer =  VirtualAlloc(NULL, dataAndFooter, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);	//page alloc

		return (page*)pointer;
	}

	void freePage(page* pointer)
	{
		VirtualFree(pointer, 0, MEM_RELEASE);
	}
	
	~FSA()
	{
	}

	void* alloc(size_t size)
	{
		void* toReturn;

		toReturn = (void*)(pages->alloc());		//сразу получили память

		if (toReturn == nullptr)					//не получили
		{
			page* p = pages->nextPage;
			while (p)			//пробуем посмотреть в других страницах
			{
				toReturn = (void*)(p->alloc());
				if (toReturn) return toReturn;

				p = p->nextPage;
			}

			page* tmp = AllocPage();		//делаем новую страницу
			tmp->initPage(sizeOfBlocks);
			tmp->nextPage = pages;
			pages = tmp;
			toReturn = (void*)(pages->alloc());
			return toReturn;

		}
		else return toReturn;

	}
	
	void free(void* p)
	{
		page* tmp = pages;

		while (tmp)
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
	

	page* pages;

	int numOfBlocks;
	int sizeOfBlocks;

	bool belongToThat(void* p)
	{
		page* tmp = pages;

		while (tmp)
		{
			auto a = (((char*)p - (char*)tmp->buffer) / sizeOfBlocks);

			if (a >= 0 && a < numOfBlocks) { return true; }
			else { tmp = tmp->nextPage; }

		}

		return false;
	}
};

