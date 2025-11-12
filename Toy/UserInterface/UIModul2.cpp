#include "pch.h"
#include "UIModul2.h"
#include "UserInterface/TextureResourceBinder/TextureResourceBinder.h"
#include "UIComponent/Traverser/UITraverser.h"
#include "UIComponent/Components/Panel.h"

using namespace UITraverser;

UIModul2::~UIModul2() = default;
UIModul2::UIModul2() :
	m_nameGen{ make_unique<UINameGenerator>() }
{}

bool UIModul2::SetupMainComponent(const UILayout& layout, const string& name,
	IRenderer* renderer, const wstring& srcBinderFilename)
{
	m_resBinder = CreateTextureResourceBinder(srcBinderFilename, renderer);
	ReturnIfFalse(m_resBinder);
	m_renderer = renderer;

	m_mainPanel = CreateComponent<Panel>(layout);
	m_mainPanel->SetUIModul2(this);
	return Rename(m_mainPanel.get(), name);
}
