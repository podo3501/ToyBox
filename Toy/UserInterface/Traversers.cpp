#include "pch.h"
#include "Traversers.h"
#include "UIComponent/Traverser/BaseTraverser.h"
#include "UIComponent/Traverser/DerivedTraverser.h"
#include "UIComponent/Traverser/NameTraverser.h"

Traversers::Traversers() :
	m_baseTraverser{ make_unique<BaseTraverser>() },
	m_derivedTraverser{ make_unique<DerivedTraverser>() },
	m_nameTraverser{ make_unique<NameTraverser>() }
{}