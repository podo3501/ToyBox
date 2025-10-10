#pragma once
#include <string>
#include <memory>
#include "Shared/System/StepTimer.h"

class Scene;
class ISceneManager
{
public:
	virtual ~ISceneManager() = default;
	virtual bool CreateScene(std::unique_ptr<Scene> scene) = 0;
	virtual bool Transition(const std::string& newSceneName) = 0;
	virtual void Update(const DX::StepTimer& timer) = 0;
	virtual Scene* FindScene(const std::string& sceneName) const noexcept = 0;
};