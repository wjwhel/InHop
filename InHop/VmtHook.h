#pragma once
#include "Windows.h"
#include <cstddef>
#include <memory>

class VmtHook {
public:
	VmtHook(void* classptr) {
		this->classptr = classptr;

		auto vmt = *reinterpret_cast<uintptr_t**>(classptr);

		std::size_t length = 0;
		MEMORY_BASIC_INFORMATION memoryInfo;
		while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
			length++;

		std::unique_ptr<uintptr_t[]> oldVmt = std::make_unique<uintptr_t[]>(length);

		std::copy(vmt, vmt + length, oldVmt.get());



	}

private:
	void* classptr = nullptr;
};