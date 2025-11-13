#pragma once
#include "Shared/Framework/Locator.h"
#include "UIComponentManager.h"
#include "UINameGenerator/UINameGenerator.h"

using UIComponentLocator = Locator<UIComponentManager>;

inline UIModul2* CreateUIModulE(const string& moduleName, const UILayout& layout, 
	const string& mainUIName, IRenderer* renderer, const wstring& srcBinderFilename = L"")
{
	return UIComponentLocator::GetService()->CreateUIModule(moduleName, layout, mainUIName, renderer, srcBinderFilename);
}

inline UIModul2* CreateUIModulE(const string& moduleName, const wstring& filename, 
	IRenderer* renderer, const wstring& srcBinderFilename = L"")
{
	return UIComponentLocator::GetService()->CreateUIModule(moduleName, filename, renderer, srcBinderFilename);
}

inline bool ReleaseUIModulE(const string& moduleName) noexcept
{
	return UIComponentLocator::GetService()->ReleaseUIModule(moduleName);
}