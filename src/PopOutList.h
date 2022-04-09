#pragma once
#include "VST2_Header.h"
#include "Allocator.h"

class CPopOutList
{
public:
	CPopOutList()
	{
		for (size_t i = 0; i < sizeof(StringsArray) / sizeof(void*); i++)
		{
			StringsArray[i] = nullptr;
		}
	}

	void AddMember(const char* String)
	{
		if (StringCount >= sizeof(StringsArray) / sizeof(void*)) return;

		// load string to string array
		size_t stringSize = strlen(String);
		StringsArray[StringCount] = (char*)heap.Alloc(stringSize + 1);
		memcpy(StringsArray[StringCount], String, stringSize);
	}

#ifdef WIN32
	void DrawList(void* pHandle, int x, int y)
	{
		HWND hwnd = (HWND)pHandle;
		HMENU hMenu = CreatePopupMenu();

		// load all items to menu
		for (size_t i = 0; i < StringCount; i++)
		{
			AppendMenuA(hMenu, MF_STRING, 0xF500 + i, StringsArray[i]);
		}

		// process by proc menu
		TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, 0, hwnd, nullptr);

		// destroy menu
		DestroyMenu(hMenu);
	}
#else

#endif

	~CPopOutList()
	{
		// free all stuff
		for (size_t i = 0; i < StringCount; i++)
		{
			heap.Delete(StringsArray[i]);
		}
	}

private:
	CHeapAllocator heap;
	char* StringsArray[96];
	size_t StringCount;
};