#include "pch.h"
#include "UIComponentManager.h"
#include "IRenderer.h"
#include "UIModule.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UIComponent/Traverser/BaseTraverser.h"
#include "UIComponent/Traverser/DerivedTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"
#include "UIComponent/Traverser/UITraverser.h"
#include "UIComponent/UILayout.h"
#include "Shared/Utils/StlExt.h"

UIComponentManager::~UIComponentManager() = default;
UIComponentManager::UIComponentManager(IRenderer* renderer, bool isTool) :
	m_renderer{ renderer },
	m_baseTraverser{ make_unique<BaseTraverser>() },
	m_derivedTraverser{ make_unique<DerivedTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>() }
{
	if(!isTool) //툴은 RenderTexture에 그리기 때문에 렌더와 연결하지 않는다.
		m_renderer->SetComponentRenderer([this](ITextureRender* r) { this->RenderComponent(r); });
	auto texController = m_renderer->GetTextureController();
	texController->SetTextureRenderer([this](size_t index, ITextureRender* r) { 
		this->RenderTextureComponent(index, r); });
}

UIModule* UIComponentManager::CreateUIModule(const string& moduleName, const UILayout& layout, 
	const string& mainUIName, const wstring& srcBinderFilename)
{
	if (m_uiModules.find(moduleName) != m_uiModules.end()) return nullptr;

	auto [owner, module] = GetPtrs(make_unique<UIModule>());
	if (!owner->SetupMainComponent(layout, mainUIName, m_renderer, srcBinderFilename)) return nullptr;
	m_uiModules.insert({ moduleName, move(owner) });

	return module;
}

UIModule* UIComponentManager::CreateUIModule(const string& moduleName, 
	const wstring& filename, const wstring& srcBinderFilename)
{
	if (m_uiModules.find(moduleName) != m_uiModules.end()) return nullptr;

	auto [owner, module] = GetPtrs(make_unique<UIModule>());
	if (!owner->SetupMainComponent(filename, m_renderer, srcBinderFilename)) return nullptr;
	m_uiModules.insert({ moduleName, move(owner) });

	return module;
}

bool UIComponentManager::ReleaseUIModule(const string& moduleName) noexcept
{
	auto it = m_uiModules.find(moduleName);
	if (it == m_uiModules.end()) return false;

	m_uiModules.erase(it);
	return true;
}

bool UIComponentManager::CreateRenderTexture(UIComponent* c, const Rectangle& targetRect,
	size_t& outIndex, UINT64* outGfxMemOffset)
{
	auto texController = m_renderer->GetTextureController();
	ReturnIfFalse(texController->CreateRenderTexture(targetRect, outIndex, outGfxMemOffset));

	auto [_, inserted] = m_renderTextures.insert({ outIndex, c });
	return inserted;
}


bool UIComponentManager::ReleaseRenderTexture(size_t index) noexcept
{
	auto it = m_renderTextures.find(index);
	if (it == m_renderTextures.end()) return false;

	m_renderTextures.erase(it);
	return true;
}

void UIComponentManager::RenderComponent(ITextureRender* render)
{
	for (auto& module : m_uiModules | views::values)
		module->Render(render);
}

void UIComponentManager::RenderTextureComponent(size_t index, ITextureRender* render)
{
	auto it = m_renderTextures.find(index);
	if (it == m_renderTextures.end()) 
		Assert(false);

	UITraverser::Render(it->second, render);
}