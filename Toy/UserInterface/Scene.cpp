#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
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
	ifstream file(filename);
	ReturnIfFalse(file.is_open());
	   
	json dataList = json::parse(file);
	for (const auto& [key, data] : dataList.items())
	{
		auto [dataType, compType] = GetType(key);
		if (dataType == DataType::Init) return false;

		auto [item, position] = GetComponent(compType, data);
		ReturnIfNullptr(item);

		m_renderItems.emplace_back(make_pair(position, move(item)));
	}

	return true;
}
