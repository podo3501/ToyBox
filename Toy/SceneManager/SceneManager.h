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

	bool CreateScene(unique_ptr<Scene> scene);
	bool Transition(const string& newSceneName);
	Scene* FindScene(const string& sceneName) const noexcept;

private:
	unordered_map<string, unique_ptr<Scene>> m_sceneList;
	Scene* m_currentScene;
};

unique_ptr<ISceneManager> CreateSceneManager();