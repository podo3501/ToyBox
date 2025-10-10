#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Toy/Interfaces/ISceneManager.h"
#include "Toy/SceneManager/SceneManager.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/MockClasses.h"

namespace System
{
	TEST_F(SceneManagerTest, CreateScene)
	{
		ISceneManager* sceneManager = Locator<ISceneManager>::GetService();

		EXPECT_TRUE(sceneManager->CreateScene(make_unique<MockScene>("MockScene1", nullptr)));
		EXPECT_TRUE(sceneManager->CreateScene(make_unique<MockScene>("MockScene2", nullptr)));

		MockScene* mockScene1 = static_cast<MockScene*>(sceneManager->FindScene("MockScene1"));
		MockScene* mockScene2 = static_cast<MockScene*>(sceneManager->FindScene("MockScene2"));
		
		//씬이 전환될때 Enter와 Leave가 제대로 호출되는지 확인한다.
		EXPECT_CALL(*mockScene1, Enter()).Times(1);
		EXPECT_CALL(*mockScene1, Leave()).Times(1);
		EXPECT_CALL(*mockScene2, Enter()).Times(1);

		EXPECT_TRUE(sceneManager->Transition("MockScene1"));
		EXPECT_TRUE(sceneManager->Transition("MockScene2"));

		EXPECT_CALL(*mockScene1, Update(::testing::_)).Times(0);
		EXPECT_CALL(*mockScene2, Update(::testing::_)).Times(1);
		DX::StepTimer timer;
		sceneManager->Update(timer);
	}
}