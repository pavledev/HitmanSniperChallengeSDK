#pragma once

#include "../TypeInfo/STypeID.h"
#include "../Resource/ZResourceStub.h"

#include <IO/BinaryReader.h>
#include <IO/BinaryWriter.h>

class HitmanSniperChallengeSDK_API ZBinaryDeserializer
{
public:
	ZBinaryDeserializer();
	void* Deserialize(const std::string& filePath, ZResourceStub* resourceStub = nullptr);
	void* Deserialize(void* buffer, const size_t size, ZResourceStub* resourceStub = nullptr);
	void* Deserialize(BinaryReader& binaryReader, ZResourceStub* resourceStub = nullptr);
	const unsigned char GetAlignment() const;

private:
	void ParseRebaseSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter);
	void HandleRebaseSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter);
	void HandleTypeReindexingSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter);
	void HandleRuntimeResourceIDReindexingSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter, ZResourceStub* resourceStub = nullptr);
	static void Align(BinaryReader& binaryReader, const size_t currentPosition, const size_t alignment);

	unsigned char alignment;
};
