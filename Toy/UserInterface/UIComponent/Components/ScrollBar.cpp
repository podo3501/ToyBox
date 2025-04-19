#include "pch.h"
#include "ScrollBar.h"
#include "ScrollSlider.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "../../JsonOperation/JsonOperation.h"
#include "Utility.h"

ScrollBar::~ScrollBar() = default;
ScrollBar::ScrollBar() :
	m_scrollBackground{ nullptr },
	m_scrollSlider{ nullptr }
{}

ScrollBar::ScrollBar(const ScrollBar& other) :
	UIComponent{ other },
	m_scrollBackground{ nullptr },
	m_scrollSlider{ nullptr },
	m_scrollPadding{ other.m_scrollPadding }
{
	ReloadDatas();
}

void ScrollBar::ReloadDatas() noexcept
{
	vector<UIComponent*> componentList = GetChildComponents();
	m_scrollBackground = ComponentCast<PatchTextureStd1*>(componentList[0]);
	m_scrollSlider = ComponentCast<ScrollSlider*>(componentList[1]);
}

bool ScrollBar::operator==(const UIComponent& rhs) const noexcept
{
	ReturnIfFalse(UIComponent::operator==(rhs));

	const ScrollBar* o = static_cast<const ScrollBar*>(&rhs);
	ReturnIfFalse(tie(m_scrollPadding) == tie(o->m_scrollPadding));
	ReturnIfFalse(EqualComponent(m_scrollBackground, o->m_scrollBackground));
	ReturnIfFalse(EqualComponent(m_scrollSlider, o->m_scrollSlider));

	return true;
}

unique_ptr<UIComponent> ScrollBar::CreateClone() const
{
	return unique_ptr<ScrollBar>(new ScrollBar(*this));
}

bool ScrollBar::Setup(const UILayout& layout, unique_ptr<UIComponent> scrollBackground, 
	unique_ptr<UIComponent> scrollSlider) noexcept
{
	SetLayout(layout);

	m_scrollBackground = ComponentCast<PatchTextureStd1*>(scrollBackground.get());
	UIEx(this).AttachComponent(move(scrollBackground), {});

	const auto& size = m_scrollBackground->GetSize();
	XMINT2 centerPos = XMUINT2ToXMINT2({ size / 2 });

	m_scrollSlider = ComponentCast<ScrollSlider*>(scrollSlider.get());
	m_scrollSlider->ChangeOrigin(Origin::Center);
	m_scrollPadding = size.y - m_scrollSlider->GetSize().y;
	UIEx(this).AttachComponent(move(scrollSlider), centerPos);

	return true;
}

bool ScrollBar::ChangeSliderSize(const XMUINT2& size) noexcept
{
	return m_scrollSlider->ChangeSize({ size.x - m_scrollPadding, size.y - m_scrollPadding });
}

bool ScrollBar::ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept
{
	const auto& size = m_scrollBackground->GetSize();
	ChangeSliderSize(size);
	return true;
}

bool ScrollBar::ImplementChangeSize(const XMUINT2& size) noexcept
{
	m_scrollBackground->ChangeSize(size);
	ChangeSliderSize(size);
	return UIComponent::ImplementChangeSize(size);
}

void ScrollBar::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
	operation.Process("ScrollPadding", m_scrollPadding);

	if (operation.IsWrite()) return;
	ReloadDatas();
}

bool ScrollBar::ToolUpdateScrollView() noexcept
{
	bool isChange = false;
	m_scrollSlider->SetViewContent(1, 2);
	return isChange;
}

bool ScrollBar::UpdateScrollView(uint32_t viewArea, uint32_t contentSize) noexcept
{
	if (IsToolMode(this)) return ToolUpdateScrollView();

	bool isChange = SetStateFlag(StateFlag::Active, contentSize > viewArea );
	m_scrollSlider->SetViewContent(viewArea, contentSize);
	return isChange;
}

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout, unique_ptr<UIComponent> scrollBackground,
	unique_ptr<UIComponent> scrollSlider)
{
	unique_ptr<ScrollBar> scrollBar = make_unique<ScrollBar>();
	return CreateIfSetup(move(scrollBar), layout, move(scrollBackground), move(scrollSlider));
}
