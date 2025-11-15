#include "pch.h"
#include "UIComponentManager.h"
#include "IRenderer.h"
#include "UIModule.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UIComponent/Traverser/BaseTraverser.h"
#include "UIComponent/Traverser/DerivedTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"
#include "UIComponent/UILayout.h"
#include "Shared/Utils/StlExt.h"

UIComponentManager::~UIComponentManager() = default;
UIComponentManager::UIComponentManager() :
	m_baseTraverser{ make_unique<BaseTraverser>() },
	m_derivedTraverser{ make_unique<DerivedTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>() }
{}

UIModule* UIComponentManager::CreateUIModule(const string& moduleName, const UILayout& layout, 
	const string& mainUIName, IRenderer* renderer, const wstring& srcBinderFilename)
{
	if (m_uiModules.find(moduleName) != m_uiModules.end()) return nullptr;

	auto [owner, module] = GetPtrs(make_unique<UIModule>());
	if (!owner->SetupMainComponent(layout, mainUIName, renderer, srcBinderFilename)) return nullptr;
	m_uiModules.insert({ moduleName, move(owner) });

	return module;
}

UIModule* UIComponentManager::CreateUIModule(const string& moduleName, const wstring& filename,
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	if (m_uiModules.find(moduleName) != m_uiModules.end()) return nullptr;

	auto [owner, module] = GetPtrs(make_unique<UIModule>());
	if (!owner->SetupMainComponent(filename, renderer, srcBinderFilename)) return nullptr;
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