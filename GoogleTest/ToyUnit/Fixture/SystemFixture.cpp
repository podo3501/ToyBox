#include "pch.h"
#include "SystemFixture.h"
#include "Shared/Utils/GeometryExt.h"
#include "Shared/Framework/Environment.h"
#include "Toy/Locator/SceneLocator.h"
#include "Toy/UserInterface/UIComponent/UIType.h"
#include "Toy/UserInterface/UIComponent/UILayout.h"

using ::testing::_;
using ::testing::Invoke;

SystemFixture::~SystemFixture() = default;
SystemFixture::SystemFixture() = default;

void SystemFixture::SetUp()
{
	Rectangle rc{ 0, 0, 800, 600 };
	UILayout layout{ GetSizeFromRectangle(rc), Origin::LeftTop };
	wstring srcBinderFilename = L"UI/SampleTexture/SampleTextureBinder.json";
	InitializeEnvironment(L"../Resources/", { 800.f, 600.f });
	m_mockRenderer = make_unique<MockRenderer>();

	m_sceneManager = CreateSceneManager();
	SceneLocator::Provide(m_sceneManager.get());
}

void SystemFixture::TearDown()
{}