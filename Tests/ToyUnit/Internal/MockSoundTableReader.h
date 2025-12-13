#pragma once
#include "Shared/System/Audio/SoundTableReader.h"

class MockSoundTableReader : public SoundTableReader
{
public:
	virtual SoundInfo* GetInfo(const string& index) noexcept override;
	bool SetInfo(const string& index, const SoundInfo& sndInfo) noexcept;

private:
	unordered_map<string, SoundInfo> m_infos;
};

SoundInfo* MockSoundTableReader::GetInfo(const string& index) noexcept
{
	auto it = m_infos.find(index);
	if (it == m_infos.end()) return nullptr;

	return &(it->second);
}

bool MockSoundTableReader::SetInfo(const string& index, const SoundInfo& sndInfo) noexcept
{
	auto it = m_infos.find(index);
	if (it != m_infos.end()) return false;

	m_infos.insert({ index, sndInfo });
	return true;
}