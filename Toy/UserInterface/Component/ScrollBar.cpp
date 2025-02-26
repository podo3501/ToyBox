#include "pch.h"
#include "ScrollBar.h"
#include "../JsonOperation.h"

ScrollBar::~ScrollBar() = default;
ScrollBar::ScrollBar() :
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{}

ScrollBar::ScrollBar(const ScrollBar& other) :
	UIComponent{ other },
	m_scrollTrack{ nullptr },
	m_scrollButton{ nullptr }
{
}

bool ScrollBar::Setup(const UILayout& layout, unique_ptr<UIComponent> scrollTrack, unique_ptr<UIComponent> scrollButton)
{
	SetLayout(layout);
	m_scrollTrack = scrollTrack.get();
	UIEx(this).AttachComponent(move(scrollTrack), {});
	
	m_scrollButton = scrollButton.get();
	UIEx(this).AttachComponent(move(scrollButton), {});

	return true;
}

void ScrollBar::SerializeIO(JsonOperation& operation)
{
	UIComponent::SerializeIO(operation);
}

unique_ptr<UIComponent> ScrollBar::CreateClone() const
{
	return unique_ptr<ScrollBar>(new ScrollBar(*this));
}

unique_ptr<UIComponent> CreateScrollBar(const UILayout& layout, 
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollButton)
{
	unique_ptr<ScrollBar> scrollBar = make_unique<ScrollBar>();
	if (!scrollBar->Setup(layout, move(scrollTrack), move(scrollButton))) return nullptr;
	return scrollBar;
}