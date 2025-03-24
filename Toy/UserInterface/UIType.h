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
	ScrollBar,
	ScrollSlider,
	ImageSwitcher,
	Unknown //�θ� �⺻ UIComponent. �̰� ������ �Ǹ� UICompnent�� ȥ�ڼ� �����Ǿ��ٴ� �ǵ�, �����ڰ� protected���� public�� �Ǿ��� ���̴�. �ƴϸ� enum���� �߰��� �Ƚ��״���.
};

template<>
constexpr size_t EnumSize<ComponentID>() { return 14; }

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
		{ "ScrollBar" },
		{ "ScrollSlider" },
		{ "ImageSwitcher" },
		{ "Unknown" }
	} };
}

enum class ImagePart : int
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

enum class BehaviorMode : int //�����̳��� ���۹��
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

//bit enum�� ���ø�ȭ �ؼ� �ٸ� bit enum�鵵 �����ϰ� �Լ��� ����ϰԲ� �Ѵ�.
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

	// SetBitEnum & HasBitEnum �Լ� ���ø�ȭ
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

// namespac + enum�� ���� ������ �Լ� ���ڷ� int�� ���� �ͺ��� ���Ƽ�. enum�� ���� ���� �̸��� ���ؼ� �ڲ� �̸� �浹�ȴ�.
// enum class�� ���۷����� �Լ��� ���� �����ǰ� Ÿ�� ��ȯ�� ��� �� ��� �ؼ� ������.
namespace StateFlag
{
	enum Type : int //������ 32������ ����. 
	{
		Update = 1 << 0, //��ġ ������Ʈ �� ActiveUpdate����
		ActiveUpdate = 1 << 1, //Active������Ʈ(Update�Լ� �����ϴ°� �ƴѰ�) ���࿩��
		Render = 1 << 2, //Render��� RenderTexture�� ���ÿ� ������ �� �ִ�. RenderTexture�� �׷��� ���� Render�Ҷ� �����ش�.
		RenderTexture = 1 << 3, //RenderTexture��� Texture�����϶��� �׸���.
		RenderEditable = 1 << 4, //ȭ�鿡 ���������� ������ Tool������ �����ٶ� ����Ѵ�. prototype ui ���� ���.
		Attach = 1 << 5,
		Detach = 1 << 6,
		X_SizeLocked = 1 << 7,
		Y_SizeLocked = 1 << 8,

		Active = Update | ActiveUpdate | Render,
		Default = Active | Attach | Detach, // �⺻ �ɼ�(��� �ɼ� ����)
	};

	constexpr StateFlag::Type operator|(StateFlag::Type lhs, StateFlag::Type rhs) { return static_cast<StateFlag::Type>(static_cast<int>(lhs) | static_cast<int>(rhs)); }
}

enum class DirectionType
{
	Horizontal,
	Vertical
};

/////////////////////////////////////////////////////////////// 

class JsonOperation;
struct ImageSource //?!? TextureSourceInfo�� ����ϰ� ������. ��ĥ���� ����
{
	bool operator==(const ImageSource& o) const noexcept { return tie(filename, list) == tie(o.filename, o.list); }
	void SerializeIO(JsonOperation& operation);
	wstring filename{};
	vector<Rectangle> list;
};

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