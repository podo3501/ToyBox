#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "IRenderItem.h"
#include "JsonHelper.h"

using json = nlohmann::json;

Scene::~Scene() = default;
Scene::Scene(IRenderer* renderer) :
	m_renderer{ renderer }
{}

bool Scene::LoadScene(ILoadData* load)
{
	return ranges::all_of(m_renderItems, [load](const auto& item) {
		return item.second->LoadResources(load);
		});
}

void Scene::RenderScene(IRender* render)
{
	ranges::for_each(m_renderItems, [render](const auto& item) {
		item.second->Render(render);
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

		auto [item, position] = GetComponent(data);
		ReturnIfNullptr(item);

		AddRenderItem(position, move(item));
	}

	return true;
}

void Scene::AddRenderItem(const Vector2& position, unique_ptr<IRenderItem> item)
{
	m_renderItems.emplace_back(make_pair(position, move(item)));
}

bool Scene::Update(const Mouse::ButtonStateTracker* mouseTracker)
{
	return ranges::all_of(m_renderItems, [mouseTracker](const auto& item) {
		return item.second->Update(item.first, mouseTracker);
		});
}

IRenderItem* Scene::GetRenderItem(const string& name)
{
	auto find = ranges::find_if(m_renderItems, [&name](const auto& item) {
		return item.second->GetName() == name;
		});
	if (find == m_renderItems.end()) return nullptr;

	return find->second.get();
}

Scene::Scene(const Scene& other)
{
	m_renderer = other.m_renderer;
	ranges::transform(other.m_renderItems, back_inserter(m_renderItems), [](const auto& item) {
		auto clone = item.second->Clone();
		return make_pair(item.first, move(clone));
		});
}

unique_ptr<IRenderScene> Scene::Clone()
{
	return make_unique<Scene>(*this);
}
