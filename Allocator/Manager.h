#pragma once
#include "cassert"
#include "Windows.h"
#include <iostream>
#include "FSA.h"
#include "CA.h"
#include "map"
#include <string>
#include <utility>

#ifdef _DEBUG
struct info
{
	
	std::map<int, int> FSAalsTosize;
	std::map<void*, int> CAtoAdresses;
	std::map<  void*, int> OSAdressTosize;
	std::map< void*,int> FSAdressTosize;
	std::map<int, int> SizeToCount;
	
	std::map<void*, int> AdressToSize;
	std::map<void*, std::string> PointerGoinToRemove;
};
#endif



class MemoryAllocator
{
public:
	MemoryAllocator() :destroyed(false), initialize(false), fsa16(16),
		fsa32(32), fsa64(64),
		fsa128(128), fsa256(256),
		fsa512(512), ca(){}
	~MemoryAllocator(){ if(!destroyed)destroy(); }
	void init()
	{
		initialize = true;
		fsa16.init();
		fsa32.init();
		fsa64.init();
		fsa128.init();
		fsa256.init();
		fsa512.init();
		/*
		fsa16 = new FSA(16,15);
		fsa32 = new FSA(32,10);
		fsa64 = new FSA(64,8);
		fsa128 = new FSA(128,5);
		fsa256 = new FSA(256,3);
		fsa512 = new FSA(512,1);*/

		ca.init();
	}
	void destroy(){
		assert(initialize);

		destroyed = true;
		fsa16.destroy();
		fsa32.destroy();
		fsa64.destroy();
		fsa128.destroy();
		fsa256.destroy();
		fsa512.destroy();
		ca.destroy();
	}
	void* alloc(size_t size)
	{
		assert(initialize);

		if (size <= 16)
		{
			auto a = fsa16.alloc(size); 

#ifdef _DEBUG
			info.FSAalsTosize[16] += size;
			info.SizeToCount[16] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 16;
#endif

			return a;
		}
		else if (size <= 32) {
			auto a = fsa32.alloc(size);
#ifdef _DEBUG
			info.FSAalsTosize[32] += size;
			info.SizeToCount[32] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 32;
#endif
			return a;
		}
		else if (size <= 64) {
			auto a = fsa64.alloc(size);
#ifdef _DEBUG
			
			info.FSAalsTosize[64] += size;
			info.SizeToCount[64] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 64;
#endif
			return a;
		}
		else if (size <= 128) {
			auto a = fsa128.alloc(size);
#ifdef _DEBUG
			info.FSAalsTosize[128] += size;
			info.SizeToCount[128] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 128;
#endif
			return a;
			
		}
		else if (size <= 256) {
			auto a = fsa256.alloc(size);
#ifdef _DEBUG
			info.FSAalsTosize[256] += size;
			info.SizeToCount[256] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 256;
#endif
			return a;
		}
		else if (size <= 512) {
			auto a = fsa16.alloc(size);
#ifdef _DEBUG
			info.FSAalsTosize[512] += size;
			info.SizeToCount[512] += 1;
			info.FSAdressTosize[a] = size;
			info.AdressToSize[a] = 512;
#endif
			return a;
		}
		else if (size <= 10 * 1024 *1024)
		{
			
			auto a =ca.alloc(size);
			
#ifdef _DEBUG
			info.CAtoAdresses[a] = size; info.SizeToCount[size] += 1;
			info.AdressToSize[a] = size;
#endif

			return a;
		}
		else
		{
			
			auto a = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);//do

			assert(a);
#ifdef _DEBUG
			info.OSAdressTosize[a] = size; info.SizeToCount[size] += 1;
			info.AdressToSize[a] = size;
#endif

			return a;
		}


		return nullptr;
	}
	
	void free(void* p)
	{
		assert(initialize);

		if (this->fsa16.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa16";
#endif
			fsa16.free(p);
		}
		else if (this->fsa32.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa32" ;
#endif
			fsa32.free(p);
		}
		else if (this->fsa64.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa64";
#endif
			fsa64.free(p);
		}
		else if (this->fsa128.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa128";
#endif
			fsa128.free(p);
		}
		else if (this->fsa256.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa256";
#endif
			fsa256.free(p);
		}
		else if (this->fsa512.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "fsa512";
#endif
			fsa512.free(p);
		}
		else if (ca.belongToThat(p))
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "Coales";
#endif
			ca.free(p);
		}
		else
		{
#ifdef _DEBUG
			info.PointerGoinToRemove[p] = "OS ";
#endif
			VirtualFree(p, 0, MEM_RELEASE);


		}

	}

#ifdef _DEBUG
	
	void dumpStat()const
	{
		std::cout << "============================================================================\n";
		for (auto a : info.SizeToCount)
		{
			std::cout << " block sized: " << a.first << " bytes was allocated " << a.second << " times.\n";
		}
		for (auto a : info.AdressToSize) std::cout << "At " << a.first << " adress located " << a.second << " bytes. \n";
		for (auto a : info.PointerGoinToRemove) std::cout << "Pointer " << a.first << " was removed by " << a.second << " allocator \n";
		std::cout << "============================================================================\n";
				
	}
	void dumpBlocks() const
	{
		std::cout << "============================================================================\n";
		for(auto a : info.SizeToCount)
		{
			std::cout << "Block sized: " << a.first << " bytes was allocated " << a.second << " times.\n";
		}
		for (auto a : info.FSAdressTosize) std::cout <<"at "<< a.first << " allocated " << a.second << " bytes by FS alloc\n";
		for (auto a : info.CAtoAdresses) std::cout <<"At "<< a.first << " allocated " << a.second << " bytes by Coalese alloc\n";
		for(auto a :info.OSAdressTosize) std::cout << a.first << " bytes was allocated in " << a.second << " by OS\n";
		std::cout << "============================================================================\n";
	}
#endif

	FSA fsa16;
	FSA fsa32;
	FSA fsa64;
	FSA fsa128;
	FSA fsa256;
	FSA fsa512;
	CA ca;
	
	bool destroyed;
	bool initialize;
	info info;
};

