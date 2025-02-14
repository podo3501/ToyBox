#pragma once
#include "EnumUtility.h"

enum class ComponentID : int
{
	Panel,
	ImageGrid1,
	ImageGrid3,
	ImageGrid9,
	Button,
	TextArea,
	Dialog,
	ListArea,
	Container,
	DrawTexture,
	Unknown //부모 기본 UIComponent. 이게 문제가 되면 UICompnent가 혼자서 생성되었다는 건데, 생성자가 protected에서 public이 되었을 것이다.
};

template<>
constexpr size_t EnumSize<ComponentID>() { return 11; }

template<>
constexpr auto EnumToStringMap<ComponentID>() -> array<const char*, EnumSize<ComponentID>()> {
	return { {
		{ "Panel" },
		{ "ImageGrid1" },
		{ "ImageGrid3" },
		{ "ImageGrid9" },
		{ "Button" },
		{ "TextArea" },
		{ "Dialog" },
		{ "ListArea" },
		{ "Container" },
		{ "DrawTexture" },
		{ "Unknown" }
	} };
}

///////////////////////////////////////////////////////////////

enum class Origin : int
{
	Init = 0,
	Center,
	LeftTop,
	Count,
};

template<>
constexpr size_t EnumSize<Origin>() { return 4; }

template<>
constexpr auto EnumToStringMap<Origin>() -> array<const char*, EnumSize<Origin>()> {
	return { {
		{ "Init" },
		{ "Center" },
		{ "LeftTop" },
		{ "Count" }
	} };
}

///////////////////////////////////////////////////////////////

enum class ButtonState : int
{
	Normal,
	Hover,
	Pressed
};

template<>
constexpr size_t EnumSize<ButtonState>() { return 3; }

template<>
constexpr auto EnumToStringMap<ButtonState>()->array<const char*, EnumSize<ButtonState>()> {
	return { {
		{ "Normal" },
		{ "Hover" },
		{ "Pressed" }
	} };
}
///////////////////////////////////////////////////////////////

//attachment할때 상태에 따라서 attach detach 할수 있는지 확인하는 값
enum class AttachmentState : int
{
	All,
	Attach, 
	Detach,
	Disable
};

///////////////////////////////////////////////////////////////

struct ImageSource
{
	wstring filename{};
	vector<Rectangle> list;
};

struct SourceDivider
{
	SourceDivider() = default;
	SourceDivider(const Rectangle& _rect, const vector<int>& _list) noexcept :
		rect{ _rect }, list{ _list } {}
	SourceDivider& operator=(const SourceDivider&) = default;
	bool operator==(const SourceDivider& other) const
	{
		return rect == other.rect && list == other.list;
	}

	Rectangle rect{};
	vector<int> list;
};