#include "pch.h"
#include "SystemFixture.h"
#include "Shared/Framework/Locator.h"
#include "Toy/SceneManager/SceneManager.h"

using ::testing::_;
using ::testing::Invoke;

SystemFixture::~SystemFixture() = default;
SystemFixture::SystemFixture() = default;

void SystemFixture::SetUp()
{
	m_sceneManager = CreateSceneManager();
	Locator<ISceneManager>::Provide(m_sceneManager.get());
}

void SystemFixture::TearDown()
{}