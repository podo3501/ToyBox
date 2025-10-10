#pragma once
#include "Mocks/MockRenderer.h"
#include "Shared/System/StepTimer.h"

class ISceneManager;
class SystemFixture : public ::testing::Test
{
public:
	SystemFixture();
	~SystemFixture();

protected:
	void SetUp() override;
	void TearDown() override;

private:
	unique_ptr<ISceneManager> m_sceneManager;
};