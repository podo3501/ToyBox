#pragma once
#include "Toy/Scenes/Scene.h"

enum MockSceneID
{
	MockScene1 = 1000,
	MockScene2,
};

class MockScene1 : public Scene
{
public:
	MockScene1(IRenderer* renderer)
		: Scene(renderer)
	{}
	static SceneID GetTypeStatic() { return static_cast<SceneID>(MockSceneID::MockScene1); }
	virtual SceneID GetTypeID() const noexcept override { return GetTypeStatic(); }

	MOCK_METHOD(bool, Enter, (), (override));
	MOCK_METHOD(bool, Leave, (), (override));
	MOCK_METHOD(void, Update, (const DX::StepTimer& timer), (override));
};

class MockScene2 : public Scene
{
public:
	MockScene2(IRenderer* renderer)
		: Scene(renderer)
	{
	}
	static SceneID GetTypeStatic() { return static_cast<SceneID>(MockSceneID::MockScene2); }
	virtual SceneID GetTypeID() const noexcept override { return GetTypeStatic(); }

	MOCK_METHOD(bool, Enter, (), (override));
	MOCK_METHOD(bool, Leave, (), (override));
	MOCK_METHOD(void, Update, (const DX::StepTimer& timer), (override));
};