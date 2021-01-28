#include "pch.h"
#include "../Allocator/Manager.h"
//#include ""
#include <list>
#include <utility>
#include <vector>


TEST(MemoryAllocatorTest, WorkTest) {
	MemoryAllocator allocator;
	allocator.init();

	int *pi = (int*)allocator.alloc(sizeof(int));
	double *pd = (double*)allocator.alloc(sizeof(double));
	int *pa = (int*)allocator.alloc(10 * sizeof(int));


	allocator.dumpBlocks();
	allocator.dumpStat();
	allocator.free(pa);
	allocator.free(pd);
	allocator.free(pi);

	allocator.destroy();
}

TEST(MemoryAllocatorTest, DifrentAllocs)
{
	MemoryAllocator allocator;
	allocator.init();

	auto a1 = allocator.alloc(15);
	auto a2 = allocator.alloc(30);
	auto a3 = allocator.alloc(60);
	auto a4 = allocator.alloc(120);
	auto a5 = allocator.alloc(250);
	auto a6 = allocator.alloc(500);
	auto a7 = allocator.alloc(1000);
	auto a8 = allocator.alloc(150000);
	

	allocator.free(a1);
	allocator.free(a2);
	allocator.free(a3);
	allocator.free(a4);
	allocator.free(a5);
	allocator.free(a6);
	allocator.free(a7);

	allocator.dumpBlocks();
	allocator.dumpStat();

	
	allocator.destroy();
}

TEST(MemoryAllocatorTest, IterativesElements)
{
	MemoryAllocator allocator;
	allocator.init();

	std::list<void*> list;
	for(int i =0 ;i< 30;i++)
	{
		auto a = allocator.alloc(50 );
		*((int*)a) = i;
		list.push_back(a);
	}

	for(auto a : list)
	{
			allocator.free(a);
	}

	allocator.dumpBlocks();
	allocator.dumpStat();
	allocator.destroy();
}

TEST(MemoryAllocatorTest, IterativesElements2)
{
	MemoryAllocator allocator;
	allocator.init();

	std::list<void*> list;
	for (int i = 0; i < 40; i++)
	{
		auto a = allocator.alloc(600);
		*((int*)a) = i;
		list.push_back(a);
	}


	
	for (auto a : list)
	{
		allocator.free(a);
	}


	allocator.dumpBlocks();
	allocator.dumpStat();
	allocator.destroy();
}

TEST(MemoryAllocatorTest, OS_ALLOC)
{
	MemoryAllocator allocator;
	allocator.init();
	
		auto a = allocator.alloc( 10500000);
		auto b = allocator.alloc(10500000);
		auto c = allocator.alloc(10500000);
		auto d = allocator.alloc(10500000);
		auto e = allocator.alloc(10500000);
	
		allocator.free(a);
		allocator.free(b);
		allocator.free(c);
		allocator.free(d);
		allocator.free(e);
	
	

	allocator.dumpBlocks();
	allocator.dumpStat();
	allocator.destroy();
}


TEST(MemoryAllocatorTest, CAFragmentation)
{
	MemoryAllocator allocator;
	allocator.init();

	std::vector<void*> list;
	for (int i = 0; i < 50; i++)
	{
		auto a = allocator.alloc(i + 600);
		list.push_back(a);
	}

	for (int i = 0; i < 48; i += 6)
	{
		allocator.free(list[i]);
		allocator.free(list[i+1]);
	}
	allocator.dumpStat();

	for (int i = 0; i < 7; i++)
	{
		auto a = allocator.alloc(i + 600);
		list.push_back(a);
	}
	allocator.dumpStat();
	
	for (auto a : list)
	{
		allocator.free(a);
	}


//	allocator.dumpBlocks();
//	allocator.dumpStat();
	allocator.destroy();
}