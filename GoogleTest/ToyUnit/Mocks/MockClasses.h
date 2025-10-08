#pragma once
#include "Toy/SceneManager/Scene.h"

class MockScene : public Scene
{
public:
	MockScene(const std::string& name)
		: Scene(name)
	{}

	MOCK_METHOD(bool, Enter, (), (override));
	MOCK_METHOD(bool, Leave, (), (override));
};