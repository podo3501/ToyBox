#include "pch.h"
#include "SceneManager.h"
#include "Scenes/Scene.h"

SceneManager::~SceneManager() = default;
SceneManager::SceneManager() :
	m_currentScene{ nullptr }
{}

bool SceneManager::CreateScene(unique_ptr<Scene> scene)
{
	string name = scene->GetName();
	auto result = m_sceneList.insert({ name, move(scene) });
	return result.second;
}

bool SceneManager::Transition(const string& newSceneName)
{
	if (m_currentScene && m_currentScene->GetName() == newSceneName)
		return true;

	if (m_currentScene)
		m_currentScene->Leave();

	Scene* newScene = FindScene(newSceneName);
	if (!newScene) return false;

	m_currentScene = newScene;
	m_currentScene->Enter();

	return true;
}

void SceneManager::Update(const DX::StepTimer& timer)
{
	if (!m_currentScene) return;
	m_currentScene->Update(timer);
}

Scene* SceneManager::FindScene(const string& sceneName) const noexcept
{
	auto find = m_sceneList.find(sceneName);
	if (find == m_sceneList.end()) return nullptr;

	return find->second.get();
}

unique_ptr<ISceneManager> CreateSceneManager()
{
	return make_unique<SceneManager>();
}
