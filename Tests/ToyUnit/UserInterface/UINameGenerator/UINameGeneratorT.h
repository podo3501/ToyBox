#pragma once
#include "Toy/UserInterface/UINameGenerator/UINameGenerator.h"
#include "Shared/Framework/EnvironmentLocator.h"

class UINameGeneratorT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};
	inline wstring GetTempDir() const noexcept { return L"../Resources/Test/Temp/"; }

	unique_ptr<Environment> m_environment; //파일 세이브 할때 필요함.
	unique_ptr<UINameGenerator> m_nameGen;
};

void UINameGeneratorT::SetUp()
{
	m_environment = InitializeEnvironment(L"", { 800.f, 600.f });
	m_nameGen = make_unique<UINameGenerator>();
}
