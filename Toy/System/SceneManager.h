#pragma once
#include "Toy/Interfaces/ISceneManager.h"

class Scene;
class SceneManager : public ISceneManager
{
public:
	~SceneManager();
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;

	virtual bool Transition(unique_ptr<Scene> newScene) override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	unique_ptr<Scene> m_currentScene;
};

unique_ptr<ISceneManager> CreateSceneManager();