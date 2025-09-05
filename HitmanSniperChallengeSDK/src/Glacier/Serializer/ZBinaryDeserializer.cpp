#include <sstream>

#include <Glacier/Serializer/ZBinaryDeserializer.h>
#include <Glacier/Serializer/BinaryBulkDataHeader.h>
#include <Glacier/TypeInfo/ZTypeRegistry.h>

#include "Logger.h"

ZBinaryDeserializer::ZBinaryDeserializer()
{
	alignment = 0;
}

void* ZBinaryDeserializer::Deserialize(const std::string& filePath, ZResourceStub* resourceStub)
{
	BinaryReader binaryReader = BinaryReader(filePath);

	return Deserialize(binaryReader);
}

void* ZBinaryDeserializer::Deserialize(void* buffer, const size_t size, ZResourceStub* resourceStub)
{
	BinaryReader binaryReader = BinaryReader(buffer, size);

	return Deserialize(binaryReader);
}

void* ZBinaryDeserializer::Deserialize(BinaryReader& binaryReader, ZResourceStub* resourceStub)
{
	unsigned int magic = binaryReader.Read<unsigned int>();

	if (magic != '1NIB')
	{
		Logger::GetInstance().Log(Logger::Level::Error, "File format not supported!");

		return nullptr;
	}

	BinaryBulkDataHeader::EDataEndianness dataEndianness = static_cast<BinaryBulkDataHeader::EDataEndianness>(binaryReader.Read<unsigned char>());
	const unsigned char alignment = binaryReader.Read<unsigned char>();
	const unsigned char sectionsCount = binaryReader.Read<unsigned char>();
	const unsigned char unusedByte = binaryReader.Read<unsigned char>();

	const unsigned char dataLength0 = binaryReader.Read<unsigned char>();
	const unsigned char dataLength1 = binaryReader.Read<unsigned char>();
	const unsigned char dataLength2 = binaryReader.Read<unsigned char>();
	const unsigned char dataLength3 = binaryReader.Read<unsigned char>();
	const unsigned int dataLength = (dataLength0 << 24) + (dataLength1 << 16) + (dataLength2 << 8) + dataLength3;

	unsigned int unusedDWORD = binaryReader.Read<unsigned int>();

	void* data = operator new(dataLength, std::align_val_t(alignment));

	binaryReader.Read(data, dataLength);

	BinaryReader dataSectionBinaryReader(data, dataLength);
	BinaryWriter dataSectionBinaryWriter(data, dataLength);

	for (unsigned char i = 0; i < sectionsCount; ++i)
	{
		const unsigned int sectionType = binaryReader.Read<unsigned int>();
		const unsigned int sectionSize = binaryReader.Read<unsigned int>();

		switch (sectionType)
		{
		case 0x12EBA5ED:
			HandleRebaseSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter);
			break;
		case 0x3989BF9F:
			HandleTypeReindexingSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter);
			break;
		case 0x578FBCEE:
			HandleRuntimeResourceIDReindexingSection(binaryReader, dataSectionBinaryReader, dataSectionBinaryWriter, resourceStub);
			break;
		default:
		{
			std::stringstream stream;

			stream << std::hex << sectionType;

			throw std::invalid_argument(std::format("Unknown section type: 0x{}!", stream.str()));
		}
		}
	}

	this->alignment = alignment;

	return data;
}

const unsigned char ZBinaryDeserializer::GetAlignment() const
{
	return alignment;
}

void ZBinaryDeserializer::HandleRebaseSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter)
{
	const unsigned int numberOfRebaseLocations = binaryReader.Read<unsigned int>();

	for (unsigned int i = 0; i < numberOfRebaseLocations; ++i)
	{
		const unsigned int rebaseLocationOffset = binaryReader.Read<unsigned int>();

		dataSectionbinaryReader.Seek(rebaseLocationOffset, SeekOrigin::Begin);
		dataSectionBinaryWriter.Seek(rebaseLocationOffset, SeekOrigin::Begin);

		const intptr_t value = dataSectionbinaryReader.Read<int>();

		if (value != -1)
		{
			dataSectionBinaryWriter.Write(reinterpret_cast<uintptr_t>(dataSectionbinaryReader.GetBuffer()) + value);
		}
		else
		{
			dataSectionBinaryWriter.Write(0);
		}
	}
}

void ZBinaryDeserializer::HandleTypeReindexingSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader, BinaryWriter& dataSectionBinaryWriter)
{
	size_t sectionStartPosition = binaryReader.GetPosition();
	unsigned int numberOfOffsetsToReindex = binaryReader.Read<unsigned int>();
	std::unordered_map<unsigned int, size_t> typeIDsToReindex;
	unsigned int typeIDOffset = 0;

	for (unsigned int i = 0; i < numberOfOffsetsToReindex; ++i)
	{
		typeIDOffset = binaryReader.Read<unsigned int>();

		dataSectionbinaryReader.Seek(typeIDOffset, SeekOrigin::Begin);

		const unsigned int typeIDIndex = dataSectionbinaryReader.Read<unsigned int>();

		typeIDsToReindex.insert(std::make_pair(typeIDOffset, typeIDIndex));
	}

	const unsigned int numberOfTypeNames = binaryReader.Read<unsigned int>();
	std::vector<STypeID*> typeIDs(numberOfTypeNames);

	for (unsigned int i = 0; i < numberOfTypeNames; ++i)
	{
		size_t currentPosition = binaryReader.GetPosition() - sectionStartPosition;

		Align(binaryReader, currentPosition, 4);

		const unsigned int typeIDIndex = binaryReader.Read<unsigned int>();
		const int typeSize = binaryReader.Read<int>();
		const unsigned int typeNameLength = binaryReader.Read<unsigned int>();
		std::string typeName = binaryReader.ReadString(typeNameLength - 1);

		STypeID* typeID = (*TypeRegistry)->GetType(typeName.c_str());

		typeIDs[typeIDIndex] = typeID;
	}

	for (auto it = typeIDsToReindex.begin(); it != typeIDsToReindex.end(); it++)
	{
		const unsigned int typeIDOffset = it->first;
		const size_t typeIDIndex = it->second;

		dataSectionBinaryWriter.Seek(typeIDOffset, SeekOrigin::Begin);
		dataSectionBinaryWriter.Write(reinterpret_cast<uintptr_t>(typeIDs[typeIDIndex]));
	}
}

void ZBinaryDeserializer::HandleRuntimeResourceIDReindexingSection(BinaryReader& binaryReader, BinaryReader& dataSectionbinaryReader,
	BinaryWriter& dataSectionBinaryWriter, ZResourceStub* resourceStub)
{
	if (!resourceStub)
	{
		return;
	}

	const unsigned int numberOfOffsetsToReindex = binaryReader.Read<unsigned int>();

	for (unsigned int i = 0; i < numberOfOffsetsToReindex; ++i)
	{
		const unsigned int runtimeResourceIDOffset = binaryReader.Read<unsigned int>();

		dataSectionbinaryReader.Seek(runtimeResourceIDOffset, SeekOrigin::Begin);

		const unsigned int idHigh = dataSectionbinaryReader.Read<unsigned int>();
		const unsigned int idLow = dataSectionbinaryReader.Read<unsigned int>(); //Index of resource reference

		if (idLow != UINT32_MAX)
		{
			const ZRuntimeResourceID& runtimeResourceID = resourceStub->GetInstallDependency(idLow).GetResourceStub()->GetRuntimeResourceID();

			dataSectionBinaryWriter.Seek(runtimeResourceIDOffset, SeekOrigin::Begin);
			dataSectionBinaryWriter.Write(runtimeResourceID);
		}
	}
}

void ZBinaryDeserializer::Align(BinaryReader& binaryReader, const size_t currentPosition, const size_t alignment)
{
	size_t misalign = currentPosition % alignment;

	if (misalign != 0)
	{
		binaryReader.Seek(alignment - misalign, SeekOrigin::Current);
	}
}
