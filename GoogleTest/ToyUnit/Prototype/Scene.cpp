#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Toy/Interfaces/ISceneManager.h"
#include "Toy/SceneManager/SceneManager.h"
#include "Mocks/MockClasses.h"

namespace Prototype
{
	TEST(SceneManager, CreateScene)
	{
		auto createSceneManager = CreateSceneManager();
		Locator<ISceneManager>::Provide(createSceneManager.get());
		ISceneManager* sceneManager = Locator<ISceneManager>::GetService();

		EXPECT_TRUE(sceneManager->CreateScene(make_unique<MockScene>("MockScene1")));
		EXPECT_TRUE(sceneManager->CreateScene(make_unique<MockScene>("MockScene2")));

		MockScene* mockScene1 = static_cast<MockScene*>(sceneManager->FindScene("MockScene1"));
		MockScene* mockScene2 = static_cast<MockScene*>(sceneManager->FindScene("MockScene2"));
		
		//���� ��ȯ�ɶ� Enter�� Leave�� ����� ȣ��Ǵ��� Ȯ���Ѵ�.
		EXPECT_CALL(*mockScene1, Enter()).Times(1);
		EXPECT_CALL(*mockScene1, Leave()).Times(1);
		EXPECT_CALL(*mockScene2, Enter()).Times(1);

		EXPECT_TRUE(sceneManager->Transition("MockScene1"));
		EXPECT_TRUE(sceneManager->Transition("MockScene2"));
	}
}