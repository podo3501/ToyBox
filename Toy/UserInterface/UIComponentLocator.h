#pragma once
#include "Shared/Framework/Locator.h"
#include "UIComponentManager.h"
#include "UINameGenerator/UINameGenerator.h"

using UIComponentLocator = Locator<UIComponentManager>;

namespace UIManager
{
	inline UINameGenerator* GetNameGenerator() noexcept {
		return UIComponentLocator::GetService()->GetNameGenerator();
	}
}