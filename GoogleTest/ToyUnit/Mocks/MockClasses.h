#pragma once
#include "Toy/Scenes/Scene.h"

class MockScene : public Scene
{
public:
	MockScene(const std::string& name, IRenderer* renderer)
		: Scene(name, renderer)
	{}

	MOCK_METHOD(bool, Enter, (), (override));
	MOCK_METHOD(bool, Leave, (), (override));
	MOCK_METHOD(void, Update, (const DX::StepTimer& timer), (override));
};