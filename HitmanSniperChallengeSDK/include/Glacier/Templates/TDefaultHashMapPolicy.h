#pragma once

#include "../ZString.h"

#include <Hash.h>

struct STypeID;

template <typename TKey>
class TDefaultHashMapPolicy
{
};

template <>
class TDefaultHashMapPolicy<ZString>
{
public:
	static unsigned int GetHashCode(const ZString& key)
	{
		return Hash::Fnv1a(key.ToCString(), key.Length());
	}
};

template <>
class TDefaultHashMapPolicy<unsigned int>
{
public:
	static unsigned int GetHashCode(const unsigned int& key)
	{
		const char* data = reinterpret_cast<const char*>(&key);
		const size_t length = sizeof(unsigned int);

		return Hash::Fnv1a(data, length);
	}
};
