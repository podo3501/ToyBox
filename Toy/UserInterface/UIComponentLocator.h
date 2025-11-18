#pragma once
#include "Shared/Framework/Locator.h"
#include "UIComponentManager.h"
#include "UINameGenerator/UINameGenerator.h"

using UIComponentLocator = Locator<UIComponentManager>;

inline UIModule* CreateUIModule(const string& moduleName, const UILayout& layout, const string& mainUIName, const wstring& srcBinderFilename = L"")
{
	return UIComponentLocator::GetService()->CreateUIModule(moduleName, layout, mainUIName, srcBinderFilename);
}

inline UIModule* CreateUIModule(const string& moduleName, const wstring& filename, const wstring& srcBinderFilename = L"")
{
	return UIComponentLocator::GetService()->CreateUIModule(moduleName, filename, srcBinderFilename);
}

inline bool ReleaseUIModule(const string& moduleName) noexcept
{
	return UIComponentLocator::GetService()->ReleaseUIModule(moduleName);
}