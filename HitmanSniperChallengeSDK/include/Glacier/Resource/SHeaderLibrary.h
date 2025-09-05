#pragma once

#include "SHeaderLibraryChunk.h"

struct SHeaderLibrary
{
	TArray<SHeaderLibraryChunk> chunks;
	TArray<ZString> externalResourceIds;
	long long ridSource;
};
