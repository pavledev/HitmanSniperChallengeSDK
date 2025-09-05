#pragma once

#include "../Templates/TArray.h"
#include "../ZString.h"

struct SHeaderLibraryChunk
{
	ZString sLibraryID;
	ZString sInstallID;
	unsigned int nOffset;
	char nFlags;
	TArray<ZString> sLanguages;
	unsigned int localizationCategory;
	TArray<int> states;
	TArray<TArray<unsigned char>> resourceHeaders;
	TArray<unsigned int> ridMappingIndices;
	TArray<unsigned long long> ridMappingIDs;
};
