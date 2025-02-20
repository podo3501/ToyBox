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
	RenderTexture,
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
		{ "RenderTexture" },
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

enum class CResult : int
{
	Failure,
	Success,
	SkipChildren,
};

// namespac + enum을 쓰는 이유는 함수 인자로 int를 쓰는 것보다 나아서. enum만 쓰면 쟤들 이름이 흔해서 자꾸 이름 충돌된다.
// enum class는 오퍼레이터 함수가 많이 생성되고 타입 변환을 계속 해 줘야 해서 귀찮다.
namespace StateFlag
{
	enum Type : int //갯수는 32개까지 가능. 
	{
		Update = 1 << 0,
		ActiveUpdate = 1 << 1,
		Render = 1 << 2, //Render라면 RenderTexture 및 Render 두군데 다 그려준다.
		RenderTexture = 1 << 3, //RenderTexture라면 Texture셋팅일때만 그린다.
		Attach = 1 << 4,
		Detach = 1 << 5,

		Active = Update | ActiveUpdate | Render,
		Default = Active | Attach | Detach, // 기본 옵션(모든 옵션 포함)
	};

	constexpr StateFlag::Type operator|(StateFlag::Type lhs, StateFlag::Type rhs) { return static_cast<StateFlag::Type>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
}


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