#include "pch.h"
#include "SceneManager.h"
#include "Scenes/Scene.h"

SceneManager::~SceneManager() = default;
SceneManager::SceneManager() :
	m_currentScene{ nullptr }
{}

bool SceneManager::Transition(unique_ptr<Scene> newScene)
{
	if (m_currentScene && m_currentScene->GetTypeID() == newScene->GetTypeID())
		return true;

	if (m_currentScene)
		m_currentScene->Leave();

	m_currentScene = move(newScene);
	m_currentScene->Enter();

	return true;
}

void SceneManager::Update(const DX::StepTimer& timer)
{
	if (!m_currentScene) return;
	m_currentScene->Update(timer);
}

unique_ptr<ISceneManager> CreateSceneManager()
{
	return make_unique<SceneManager>();
}
