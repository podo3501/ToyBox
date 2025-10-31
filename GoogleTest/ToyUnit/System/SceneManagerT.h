#pragma once
#include "Toy/Locator/SceneLocator.h"
#include "Mocks/MockRenderer.h"

class SceneManagerT : public ::testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override {};

	unique_ptr<MockRenderer> m_renderer;
	unique_ptr<ISceneManager> m_sceneManager;
};

void SceneManagerT::SetUp()
{
	m_renderer = make_unique<MockRenderer>();

	m_sceneManager = CreateSceneManager();
	SceneLocator::Provide(m_sceneManager.get());
}
