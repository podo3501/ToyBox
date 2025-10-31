#include "pch.h"
#include "SceneManagerT.h"
#include "Toy/Scenes/Scene.h"
#include "Shared/Utils/StlExt.h"

class MockScene : public Scene
{
public:
	MockScene(IRenderer* renderer, SceneID sceneId) :
		Scene(renderer),
		m_sceneID{ sceneId }
	{}

	virtual SceneID GetTypeID() const noexcept override { return m_sceneID; }

	MOCK_METHOD(bool, Enter, (), (override));
	MOCK_METHOD(bool, Leave, (), (override));
	MOCK_METHOD(void, Update, (const DX::StepTimer& timer), (override));

private:
	SceneID m_sceneID{ SceneID::Unknown };
};

namespace SystemT
{
	TEST_F(SceneManagerT, Transition)
	{
		DX::StepTimer timer;
		ISceneManager* sceneManager = SceneLocator::GetService();

		//씬이 전환될때 Enter와 Leave가 제대로 호출되는지 확인한다.
		auto [scene1, scene1Ptr] = GetPtrs(make_unique<MockScene>(m_renderer.get(), SceneID::TestScene1));
		auto [scene2, scene2Ptr] = GetPtrs(make_unique<MockScene>(m_renderer.get(), SceneID::TestScene2));

		EXPECT_CALL(*scene1Ptr, Enter()).Times(1);
		sceneManager->Transition(move(scene1));
		sceneManager->Update(timer);

		EXPECT_CALL(*scene1Ptr, Leave()).Times(1);
		EXPECT_CALL(*scene2Ptr, Enter()).Times(1);
		sceneManager->Transition(move(scene2));
		sceneManager->Update(timer);
	}
}