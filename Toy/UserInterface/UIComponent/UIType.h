#pragma once
#include "EnumUtility.h"

enum class ComponentID : int
{
	Panel,
	PatchTextureStd1,
	PatchTextureStd3,
	PatchTextureStd9,
	PatchTextureLite1,
	PatchTextureLite3,
	PatchTextureLite9,
	Button,
	TextArea,
	Dialog,
	ListArea,
	Container,
	RenderTexture,
	ScrollBar,
	TextureSwitcher,
	UIModuleAsComponent,
	Unknown //부모 기본 UIComponent. 이게 문제가 되면 UICompnent가 혼자서 생성되었다는 건데, 생성자가 protected에서 public이 되었을 것이다. 아니면 enum에서 추가를 안시켰던지.
};

template<>
constexpr size_t EnumSize<ComponentID>() { return 17; }

template<>
constexpr auto EnumToStringMap<ComponentID>() -> array<const char*, EnumSize<ComponentID>()> {
	return { {
		{ "Panel" },
		{ "PatchTextureStd1" },
		{ "PatchTextureStd3" },
		{ "PatchTextureStd9" },
		{ "PatchTextureLite1" },
		{ "PatchTextureLite3" },
		{ "PatchTextureLite9" },
		{ "Button" },
		{ "TextArea" },
		{ "Dialog" },
		{ "ListArea" },
		{ "Container" },
		{ "RenderTexture" },
		{ "ScrollBar" },
		{ "TextureSwitcher" },
		{ "UIModuleAsComponent" },
		{ "Unknown" }
	} };
}

enum class TextureSlice : int
{
	One,
	ThreeH, //Horizontal
	ThreeV, //Vertical
	Nine
};

///////////////////////////////////////////////////////////////

enum class Origin : int
{
	LeftTop,
	RightTop,
	Center,
};

template<>
constexpr size_t EnumSize<Origin>() { return 3; }

template<>
constexpr auto EnumToStringMap<Origin>() -> array<const char*, EnumSize<Origin>()> {
	return { {
		{ "LeftTop" },
		{ "RightTop" },
		{ "Center" }
	} };
}

///////////////////////////////////////////////////////////////

enum class InteractState : int
{
	Normal,
	Hovered,
	Pressed
};

template<>
constexpr size_t EnumSize<InteractState>() { return 3; }

template<>
constexpr auto EnumToStringMap<InteractState>()->array<const char*, EnumSize<InteractState>()> {
	return { {
		{ "Normal" },
		{ "Hovered" },
		{ "Pressed" }
	} };
}

enum class BehaviorMode : int //컨테이너의 동작방식
{
	Normal,
	HoldToKeepPressed,
};

///////////////////////////////////////////////////////////////

enum class CResult : int
{
	Failure,
	Success,
	SkipChildren,
};

//bit enum을 템플릿화 해서 다른 bit enum들도 동일하게 함수를 사용하게끔 한다.
namespace BitEnum
{
	template <typename EnumType>
	constexpr typename std::enable_if_t<std::is_enum_v<EnumType>, EnumType>
		operator|(EnumType lhs, EnumType rhs) {
		using UnderlyingType = std::underlying_type_t<EnumType>;
		return static_cast<EnumType>(static_cast<UnderlyingType>(lhs) | static_cast<UnderlyingType>(rhs));
	}

	template <typename EnumType>
	constexpr typename std::enable_if_t<std::is_enum_v<EnumType>, EnumType>
		operator&(EnumType lhs, EnumType rhs) {
		using UnderlyingType = std::underlying_type_t<EnumType>;
		return static_cast<EnumType>(static_cast<UnderlyingType>(lhs) & static_cast<UnderlyingType>(rhs));
	}

	template <typename EnumType>
	constexpr typename std::enable_if_t<std::is_enum_v<EnumType>, EnumType>
		operator~(EnumType flag) {
		using UnderlyingType = std::underlying_type_t<EnumType>;
		return static_cast<EnumType>(~static_cast<UnderlyingType>(flag));
	}

	// SetBitEnum & HasBitEnum 함수 템플릿화
	template <typename EnumType>
	inline bool Set(EnumType& stateFlag, EnumType flag, bool enabled) noexcept {
		EnumType previous = stateFlag;
		stateFlag = enabled ? (stateFlag | flag) : (stateFlag & ~flag);
		return previous != stateFlag;
	}

	template <typename EnumType>
	inline bool Has(EnumType stateFlag, EnumType flag) noexcept {
		return (stateFlag & flag) != 0;
	}
}

// namespac + enum을 쓰는 이유는 함수 인자로 int를 쓰는 것보다 나아서. enum만 쓰면 쟤들 이름이 흔해서 자꾸 이름 충돌된다.
// enum class는 오퍼레이터 함수가 많이 생성되고 타입 변환을 계속 해 줘야 해서 귀찮다.
namespace StateFlag
{
	enum Type : int //갯수는 32개까지 가능. 
	{
		Update = 1 << 0, //위치 업데이트 및 ActiveUpdate포함
		ActiveUpdate = 1 << 1, //Active업데이트(Update함수 실행하는가 아닌가) 실행여부. child들은 이 옵션이 false 되면 그 밑은 전부 돌지 않는다. 
		Render = 1 << 2, //Render라면 RenderTexture를 동시에 셋팅할 수 있다. RenderTexture에 그려진 것을 Render할때 보여준다.
		RenderTexture = 1 << 3, //RenderTexture라면 Texture셋팅일때만 그린다.
		Attach = 1 << 4,
		Detach = 1 << 5,
		X_SizeLocked = 1 << 6,
		Y_SizeLocked = 1 << 7,
		LockPosOnResize = 1 << 8, //사이즈를 조절할때 자식 클래스의 위치값을 변하지 않게 한다.
		Active = Update | ActiveUpdate | Render,
		Default = Active | Attach | Detach, // 기본 옵션(모든 옵션 포함)
	};

	constexpr StateFlag::Type operator|(StateFlag::Type lhs, StateFlag::Type rhs) { return static_cast<StateFlag::Type>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
}

//렌더링시 탐색을 바꿔가면서 해야한다. 그렇지 않으면 이미지가 덮혀지거나 순서가 꼬이게 된다.
enum class RenderTraversal
{
	BFS,
	DFS,
	Inherited, //윗 노드가 사용한 방식을 따라서.
};

enum class DirectionType
{
	Horizontal,
	Vertical
};

TextureSlice DirTypeToTextureSlice(DirectionType dirType) noexcept;
optional<DirectionType> TextureSliceToDirType(TextureSlice texSlice) noexcept;

/////////////////////////////////////////////////////////////// 

struct SourceDivider
{
	SourceDivider() = default;
	SourceDivider(const Rectangle& _rect, const vector<int>& _list = {}) noexcept :
		rect{ _rect }, list{ _list } {}
	SourceDivider& operator=(const SourceDivider&) = default;
	bool operator==(const SourceDivider& other) const
	{
		return rect == other.rect && list == other.list;
	}
	bool Empty() const noexcept { return rect == Rectangle{} && list.empty(); }

	Rectangle rect{};
	vector<int> list;
};