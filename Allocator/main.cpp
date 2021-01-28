#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
#include <iostream>
#include "CA.h"
#include "Manager.h"
#include "gtest/gtest.h"
#include <list>


class weight
{
	long a[1000];
};



using namespace std;


int main()
{

	int a = 1 / 0.99f;

	MemoryAllocator allocator;
	allocator.init();

	std::list<void*> list;
	for (int i = 0; i < 20; i++)
	{
		auto a = allocator.alloc(1024);
		int* aa = ((int*)a);
		/*for (int i = 0; i < 256; i++)
			aa[i] = -1;*/
		list.push_back(a);
	}

	for (auto a : list)
	{
		allocator.free(a);
	}

	allocator.dumpBlocks();
	allocator.dumpStat();
	allocator.destroy();

	return 0;
}



