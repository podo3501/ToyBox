#include "pch.h"
#include "Shared/Framework/Locator.h"
#include "Toy/Interfaces/ISceneManager.h"
#include "Toy/System/SceneManager.h"
#include "Fixture/FixtureSuite.h"
#include "Mocks/MockClasses.h"
#include "Shared/Utils/StlExt.h"

namespace System
{
	TEST_F(SceneManagerTest, CreateScene)
	{
		DX::StepTimer timer;
		ISceneManager* sceneManager = Locator<ISceneManager>::GetService();

		//���� ��ȯ�ɶ� Enter�� Leave�� ����� ȣ��Ǵ��� Ȯ���Ѵ�.
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