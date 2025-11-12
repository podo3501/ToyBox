#include "pch.h"
#include "UIComponentManager.h"
#include "IRenderer.h"
#include "UIModul2.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UIComponent/Traverser/BaseTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"
#include "UIComponent/UILayout.h"

UIComponentManager::~UIComponentManager() = default;
UIComponentManager::UIComponentManager() :
	m_baseTraverser{ make_unique<BaseTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>()) }
{}

UIModul2* UIComponentManager::CreateUIModule(const UILayout& layout, const string& mainUIName,
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_uiModule = make_unique<UIModul2>();
	if (!m_uiModule->SetupMainComponent(layout, mainUIName, renderer, srcBinderFilename)) return nullptr;

	return m_uiModule.get();
}