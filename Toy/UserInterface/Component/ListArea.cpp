#include "pch.h"
#include "ListArea.h"

ListArea::~ListArea() = default;
ListArea::ListArea() noexcept = default;

unique_ptr<UIComponent> ListArea::CreateClone() const
{
	return nullptr;
}

bool ListArea::Setup(const UILayout& layout, const ImageSource& source) noexcept
{
	SetLayout(layout);
	source;

	return true;
}

unique_ptr<ListArea> CreateListArea(const UILayout& layout, const ImageSource& source)
{
	unique_ptr<ListArea> listArea= make_unique<ListArea>();
	if (!listArea->Setup(layout, source)) return nullptr;
	return listArea;
}
