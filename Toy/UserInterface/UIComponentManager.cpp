#include "pch.h"
#include "UIComponentManager.h"
#include "UINameGenerator/UINameGenerator.h"
#include "UIComponent/Traverser/AttachmentTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"

UIComponentManager::UIComponentManager() :
	m_nameGenerator{ make_unique<UINameGenerator>() },
	m_attachmentTraverser{ make_unique<AttachmentTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>(m_nameGenerator.get()) }
{}