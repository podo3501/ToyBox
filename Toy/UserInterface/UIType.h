#pragma once
#include "EnumUtility.h"

enum class Origin : int
{
	Init = 0,
	Center,
	LeftTop,
	Count,
};

template<>
constexpr std::size_t EnumSize<Origin>() { return 4; }

template<>
constexpr auto EnumToStringMap<Origin>() -> std::array<const char*, EnumSize<Origin>()> {
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
	Init = 0,
	Normal = 1,
	Hover = 2,
	Pressed = 3,
	Count,
};

template<>
constexpr std::size_t EnumSize<ButtonState>() { return 5; }

template<>
constexpr auto EnumToStringMap<ButtonState>()->std::array<const char*, EnumSize<ButtonState>()> {
	return { {
		{ "Init" },
		{ "Normal" },
		{ "Hover" },
		{ "Pressed" },
		{ "Count" }
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
	Rectangle rect{};
	vector<int> list;
};