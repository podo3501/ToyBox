#pragma once
#include "Shared/System/Audio/SoundTableReader.h"
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/SerializerIO/Storage/JsonStorageLocator.h"

class SoundTableReaderT : public ::testing::Test
{
protected:
	virtual void SetUp() override;

	unique_ptr<Environment> m_environment;
	unique_ptr<IJsonStorage> m_jsonStorage;
	SoundTableReader m_reader;
};

void SoundTableReaderT::SetUp()
{
	m_environment = InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_jsonStorage = InitializeJsonStorage(StorageType::File);
}
	
