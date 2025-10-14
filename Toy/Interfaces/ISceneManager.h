#pragma once
#include <string>
#include <memory>
#include "Shared/System/StepTimer.h"

class Scene;
struct ISceneManager
{
	virtual ~ISceneManager() = default;
	virtual void Transition(unique_ptr<Scene> newScene) = 0;
	virtual void Update(const DX::StepTimer& timer) = 0;
};