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

	virtual bool CreateScene(unique_ptr<Scene> scene) override;
	virtual bool Transition(const string& newSceneName) override;
	virtual void Update(const DX::StepTimer& timer) override;
	virtual Scene* FindScene(const string& sceneName) const noexcept override;

private:
	unordered_map<string, unique_ptr<Scene>> m_sceneList;
	Scene* m_currentScene;
};

unique_ptr<ISceneManager> CreateSceneManager();