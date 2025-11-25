#pragma once
#include "Shared/Framework/EnvironmentLocator.h"
#include "Shared/SerializerIO/Storage/JsonStorageLocator.h"

class SerializerIOT : public ::testing::Test
{
protected:
	void SetUp() override;
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }
	template<typename T>
	bool TestSerialization(T& inputData);

private:
	unique_ptr<Environment> m_environment;
	unique_ptr<IJsonStorage> m_storage;
};

void SerializerIOT::SetUp()
{
	m_environment = InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_storage = InitializeJsonStorage(StorageType::Memory);
}
