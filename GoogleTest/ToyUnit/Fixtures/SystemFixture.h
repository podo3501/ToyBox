#pragma once
#include "Mocks/MockRenderer.h"
#include "Shared/System/StepTimer.h"

struct ISceneManager;
class SystemFixture : public ::testing::Test
{
public:
	SystemFixture();
	~SystemFixture();

protected:
	void SetUp() override;
	void TearDown() override;

protected:
	unique_ptr<MockRenderer> m_mockRenderer;

private:
	unique_ptr<ISceneManager> m_sceneManager;
};