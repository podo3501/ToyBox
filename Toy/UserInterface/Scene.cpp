#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "UIComponent.h"
#include "JsonHelper.h"
#include "JsonOperation.h"

using json = nlohmann::json;

Scene::~Scene() = default;
Scene::Scene() :
	m_mainComponent{ make_unique<UIComponent>() }
{
	m_mainComponent->SetName("Main");
}

Scene::Scene(const Scene& other)
{
	m_mainComponent = make_unique<UIComponent>(*other.m_mainComponent);
}

Scene& Scene::operator=(const Scene& other)
{
	if (this == &other) return *this;

	m_mainComponent = make_unique<UIComponent>(*other.m_mainComponent);

	return *this;
}

bool Scene::operator==(const Scene& o) const noexcept
{
	return tie(*m_mainComponent) == tie(*o.m_mainComponent);
}

unique_ptr<IRenderScene> Scene::Clone()
{
	return make_unique<Scene>(*this);
}

bool Scene::LoadScene(ILoadData* load)
{
	return m_mainComponent->LoadResources(load);
}

bool Scene::SetDatas(IGetValue* value)
{
	return m_mainComponent->SetDatas(value);
}

void Scene::RenderScene(IRender* render)
{
	m_mainComponent->Render(render);
}

bool Scene::LoadData(const wstring& filename)
{
	filename;
	return true;
}

void Scene::AddComponent(const Vector2& position, unique_ptr<UIComponent> comp)
{
	m_mainComponent->AddComponent(move(comp), position);
}

bool Scene::Update(const Mouse::ButtonStateTracker* mouseTracker)
{
	return m_mainComponent->Update({}, mouseTracker);
}

UIComponent* Scene::GetComponent(const string& name)
{
	return m_mainComponent->GetComponent(name);
}

void to_json(nlohmann::ordered_json& j, const Scene& data)
{
	DataToJson("Main", data.m_mainComponent, j);
}

void from_json(const nlohmann::json& j, Scene& data)
{
	DataFromJson("Main", data.m_mainComponent, j);
}

void Scene::SerializeIO(JsonOperation* operation)
{
	operation->Process("Component", m_mainComponent);
}
