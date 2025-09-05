#pragma once

#include <Common.h>

class ZDynamicResourceLibrary;

HitmanSniperChallengeSDK_API void* AllocateMemory(size_t size);
HitmanSniperChallengeSDK_API void FreeMemory(void* ptr);
HitmanSniperChallengeSDK_API void FreeMemory(void* ptr, size_t alignment);

template <typename T>
void FreeObject(T* ptr);

extern template HitmanSniperChallengeSDK_API void FreeObject<ZDynamicResourceLibrary>(ZDynamicResourceLibrary* ptr);
