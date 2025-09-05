#include <Glacier/Resource/ZDynamicResourceLibrary.h>

#include <Utility/MemoryUtility.h>

HitmanSniperChallengeSDK_API void* AllocateMemory(size_t size)
{
    return new char[size];
}

HitmanSniperChallengeSDK_API void FreeMemory(void* ptr)
{
    delete[] ptr;
}

HitmanSniperChallengeSDK_API void FreeMemory(void* ptr, size_t alignment)
{
    operator delete(ptr, std::align_val_t(alignment));
}

template <typename T>
void FreeObject(T* ptr)
{
    delete ptr;
}

template HitmanSniperChallengeSDK_API void FreeObject<ZDynamicResourceLibrary>(ZDynamicResourceLibrary* ptr);
