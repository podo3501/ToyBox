#include "pch.h"
#include "Scene.h"
#include "../Utility.h"
#include "../Include/IRenderer.h"
#include "Dialog.h"

Scene::~Scene() = default;
Scene::Scene(IRenderer* renderer) :
	m_renderer{ renderer }
{}

bool Scene::LoadScene(ILoadData* load)
{
	return ranges::all_of(m_items, [load](const auto& item) {
		return item->LoadResources(load);
		});
}

void Scene::RenderScene(IRender* render)
{
	ranges::for_each(m_items, [render](const auto& item) {
		item->Render(render);
		});
}

//파일을 로드하는 부분. 지금은 파일이 없이 값을 바로 넣는다.
bool Scene::LoadData()
{
	unique_ptr<Dialog> dialog = std::make_unique<Dialog>(m_renderer);
	ReturnIfFalse(dialog->SetUIItem());

	m_items.emplace_back(move(dialog));

	return true;
}

