#pragma once

#define _CONCAT2(x, y) x##y
#define CONCAT2(x, y) _CONCAT(x, y)
#define INSERT_PADDING(length) \
	uint8_t CONCAT2(pad, __LINE__)[length]

#define ASSERT_OFFSETOF(type, field, offset) \
	static_assert(offsetof(type, field) == offset, "offsetof assertion failed")

#define ASSERT_SIZEOF(type, size) \
	static_assert(sizeof(type) == size, "sizeof assertion failed")

#ifdef BASE_ADDRESS
const HMODULE MODULE_HANDLE = GetModuleHandle(nullptr);

#define ASLR(address) \
	((size_t)MODULE_HANDLE + (size_t)address - (size_t)BASE_ADDRESS)
#endif

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
	returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location)

#define PROC_ADDRESS(libraryName, procName) \
	GetProcAddress(LoadLibrary(TEXT(libraryName)), procName)

#define HOOK(returnType, callingConvention, functionName, location, ...) \
	typedef returnType callingConvention _##functionName(__VA_ARGS__); \
	_##functionName* original##functionName = (_##functionName*)(location); \
	returnType callingConvention implOf##functionName(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
	do { \
		DetourTransactionBegin(); \
		DetourUpdateThread(GetCurrentThread()); \
		DetourAttach((void**)&original##functionName, implOf##functionName); \
		DetourTransactionCommit(); \
	} while(0)

#define VTABLE_HOOK(returnType, callingConvention, className, functionName, ...) \
	typedef returnType callingConvention className##functionName(className* This, __VA_ARGS__); \
	className##functionName* original##className##functionName; \
	returnType callingConvention implOf##className##functionName(className* This, __VA_ARGS__)

#define INSTALL_VTABLE_HOOK(className, object, functionName, functionIndex) \
	do { \
		if (original##className##functionName == nullptr) \
		{ \
			original##className##functionName = (*(className##functionName***)object)[functionIndex]; \
			DetourTransactionBegin(); \
			DetourUpdateThread(GetCurrentThread()); \
			DetourAttach((void**)&original##className##functionName, implOf##className##functionName); \
			DetourTransactionCommit(); \
		} \
	} while(0)

#define WRITE_MEMORY(location, type, ...) \
	do { \
		const type data[] = { __VA_ARGS__ }; \
		DWORD oldProtect; \
		VirtualProtect((void*)(location), sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy((void*)(location), data, sizeof(data)); \
		VirtualProtect((void*)(location), sizeof(data), oldProtect, &oldProtect); \
	} while(0)

#define WRITE_NOP(location, count) \
	do { \
		DWORD oldProtect; \
		VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
		for (size_t i = 0; i < (size_t)(count); i++) \
			*((uint8_t*)(location) + i) = 0x90; \
		VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
	} while(0)

inline uint32_t readUnalignedU32(void* memory)
{
	uint8_t* p = (uint8_t*)memory;
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

inline void versionWarning(LPCTSTR modName)
{
	MessageBox(nullptr, TEXT("Failed to initialize! Please make sure that the DivaMegaMix executable is from v1.03 and try again.\nMD5: 813E1BEFAE1776D4FAFDF907E509B28B"), modName, MB_ICONERROR);
}
