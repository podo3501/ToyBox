#include "pch.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/MockClasses.h"
#include "Shared/Utils/StlExt.h"
#include "Toy/Locator/SceneLocator.h"

namespace System
{
	TEST_F(SceneManagerTest, CreateScene)
	{
		DX::StepTimer timer;
		ISceneManager* sceneManager = SceneLocator::GetService();

		//씬이 전환될때 Enter와 Leave가 제대로 호출되는지 확인한다.
		auto [scene1, scene1Ptr] = GetPtrs(make_unique<MockScene1>(m_mockRenderer.get()));
		auto [scene2, scene2Ptr] = GetPtrs(make_unique<MockScene2>(m_mockRenderer.get()));

		EXPECT_CALL(*scene1Ptr, Enter()).Times(1);
		sceneManager->Transition(move(scene1));
		sceneManager->Update(timer);

		EXPECT_CALL(*scene1Ptr, Leave()).Times(1);
		EXPECT_CALL(*scene2Ptr, Enter()).Times(1);
		sceneManager->Transition(move(scene2));
		sceneManager->Update(timer);
	}
}