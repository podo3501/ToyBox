#pragma once
#include <string>
#include <memory>
#include "Shared/System/StepTimer.h"

class Scene;
class ISceneManager
{
public:
	virtual ~ISceneManager() = default;
	virtual bool Transition(unique_ptr<Scene> newScene) = 0;
	virtual void Update(const DX::StepTimer& timer) = 0;
};