#include "pch.h"
#include "UIComponentManager.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UIComponent/Traverser/BaseTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"

UIComponentManager::~UIComponentManager() = default;
UIComponentManager::UIComponentManager() :
	m_nameGenerator{ make_unique<UINameGenerator>() },
	m_baseTraverser{ make_unique<BaseTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>(m_nameGenerator.get()) }
{}