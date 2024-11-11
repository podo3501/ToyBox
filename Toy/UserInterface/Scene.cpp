#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "UIComponent.h"
#include "JsonHelper.h"

using json = nlohmann::json;

Scene::~Scene() = default;
Scene::Scene(IRenderer* renderer) :
	m_renderer{ renderer }
{}

bool Scene::LoadScene(ILoadData* load)
{
	return ranges::all_of(m_components, [load](const auto& comp) {
		return comp.second->LoadResources(load);
		});
}

void Scene::RenderScene(IRender* render)
{
	ranges::for_each(m_components, [render](const auto& comp) {
		comp.second->Render(render);
		});
}

bool Scene::LoadData(const wstring& filename)
{
	const json& dataList = LoadUIFile(filename);
	if (dataList.is_null())
		return false;

	for (const auto& [key, data] : dataList.items())
	{
		auto dataType = GetType(key);
		if (dataType == DataType::Init) return false;

		auto [comp, position] = CreateComponent(data);
		ReturnIfNullptr(comp);

		AddComponent(position, move(comp));
	}

	return true;
}

void Scene::AddComponent(const Vector2& position, unique_ptr<UIComponent> comp)
{
	m_components.emplace_back(make_pair(position, move(comp)));
}

bool Scene::Update(const Mouse::ButtonStateTracker* mouseTracker)
{
	return ranges::all_of(m_components, [mouseTracker](const auto& comp) {
		return comp.second->Update(comp.first, mouseTracker);
		});
}

UIComponent* Scene::GetComponent(const string& name)
{
	auto find = ranges::find_if(m_components, [&name](const auto& comp) {
		return comp.second->GetName() == name;
		});
	if (find == m_components.end()) return nullptr;

	return find->second.get();
}

Scene::Scene(const Scene& other)
{
	m_renderer = other.m_renderer;
	ranges::transform(other.m_components, back_inserter(m_components), [](const auto& comp) {
		auto clone = comp.second->Clone();
		return make_pair(comp.first, move(clone));
		});
}

unique_ptr<IRenderScene> Scene::Clone()
{
	return make_unique<Scene>(*this);
}
