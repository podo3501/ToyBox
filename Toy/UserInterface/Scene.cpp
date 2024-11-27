#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "Panel.h"
#include "JsonOperation.h"
#include "../HelperClass.h"

using json = nlohmann::json;

Scene::~Scene() = default;
Scene::Scene(const Rectangle& rect) :
	m_mainPanel{ make_unique<Panel>("Main", rect) }
{}

Scene::Scene(const Scene& other)
{
	m_mainPanel = other.m_mainPanel->Clone();
}

Scene& Scene::operator=(const Scene& other)
{
	if (this == &other) return *this;

	m_mainPanel = other.m_mainPanel->Clone();

	return *this;
}

bool Scene::operator==(const Scene& o) const noexcept
{
	return tie(*m_mainPanel) == tie(*o.m_mainPanel);
}

unique_ptr<IRenderScene> Scene::Clone()
{
	return make_unique<Scene>(*this);
}

bool Scene::LoadScene(ILoadData* load)
{
	return m_mainPanel->LoadResources(load);
}

bool Scene::SetDatas(IGetValue* value)
{
	return m_mainPanel->SetDatas(value);
}

void Scene::RenderScene(IRender* render)
{
	m_mainPanel->Render(render);
}

bool Scene::LoadData(const wstring& filename)
{
	filename;
	return true;
}

void Scene::AddComponent(const Vector2& position, unique_ptr<UIComponent> comp)
{
	m_mainPanel->AddComponent(move(comp), position);
}

bool Scene::Update(CustomButtonStateTracker* mouseTracker) noexcept
{
	return m_mainPanel->Update({}, mouseTracker);
}

UIComponent* Scene::GetComponent(const string& name)
{
	return m_mainPanel->GetComponent(name);
}

void Scene::SerializeIO(JsonOperation* operation)
{
	operation->Process("Component", m_mainPanel);
}
