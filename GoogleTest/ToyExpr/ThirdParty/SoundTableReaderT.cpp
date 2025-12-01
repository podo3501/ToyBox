#include "pch.h"
#include "SoundTableReaderT.h"
#include "Shared/SerializerIO/SerializerIO.h"
#include "Shared/Utils/EnumHelpers.h"
#include "Shared/System/Public/AudioTypes.h"
#include "Shared/SerializerIO/Traits/Traits.h"
#include "Shared/SerializerIO/Traits/TraitsHelper.hpp"

template<>
constexpr size_t EnumSize<AudioGroupID>() { return 6; }

template<>
constexpr auto EnumToStringMap<AudioGroupID>() -> array<const char*, EnumSize<AudioGroupID>()> {
	return { {
		{ "Master" },
		{ "BGM" },
		{ "SFX" },
		{ "UI" },
		{ "System" },
		{ "None" }
	} };
}

DECLARE_JSON_TRAITS(AudioGroupID)

nlohmann::ordered_json JsonTraitsBase<AudioGroupID>::SerializeToJson(const AudioGroupID& data) { return EnumToString(data); }
AudioGroupID JsonTraitsBase<AudioGroupID>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return CreateAndFill<AudioGroupID>([&dataJ](AudioGroupID& data) {
		data = *StringToEnum<AudioGroupID>(dataJ); });
}

struct SoundEntry
{
	void ProcessIO(SerializerIO& serializer);

	string filename;
	AudioGroupID group{ AudioGroupID::None };
	float volume;
};

void SoundEntry::ProcessIO(SerializerIO& serializer)
{
	serializer.Process("Filename", filename);
	serializer.Process("Group", group);
	serializer.Process("Volume", volume);
};

class SoundTableReader
{
public:
	bool Read(const wstring& filename);
	void ProcessIO(SerializerIO& serializer);

private:
	unordered_map<string, SoundEntry> m_entries;
};

bool SoundTableReader::Read(const wstring& filename)
{
	auto fullFilename = GetResourceFullFilename(filename);
	return SerializerIO::ReadJsonFromFile(fullFilename, *this);
}

void SoundTableReader::ProcessIO(SerializerIO& serializer)
{
	serializer.Process("Entries", m_entries);
}

namespace ThirdParty
{
	TEST_F(SoundTableReaderT, Read)
	{
		SoundTableReader reader;
		EXPECT_TRUE(reader.Read(L"Sound/SoundTable.json"));
	}
}