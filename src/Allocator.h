#pragma once
#include "VST2_Header.h"

class CHeapAllocator
{
public:
	void* Alloc(size_t SizeOfPointer)
	{
		void* pRet = nullptr;
#ifdef WIN32
		pRet = HeapAlloc(GetProcessHeap(), 0, SizeOfPointer);
#else
		pRet = malloc(SizeOfPointer);
#endif
		memset(pRet, 0, SizeOfPointer);
		return pRet;
	}

	bool Delete(void* pPointer)
	{
		if (pPointer)
		{
			try
			{
#ifdef WIN32
				return !!HeapFree(GetProcessHeap(), 0, pPointer);
#else
				free(pPointer);
				return true;
#endif
			}
			catch (...)
			{
				return false;
			}
		}

		return false;
	}
};
