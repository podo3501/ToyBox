#pragma once

enum class Origin : int
{
	Init = 0,
	Center,
	LeftTop,
	Count,
};

template<typename EnumType>
constexpr std::size_t EnumSize();

template<>
constexpr std::size_t EnumSize<Origin>() { return 4; }

template<typename EnumType>
constexpr auto EnumToStringMap() -> std::array<const char*, EnumSize<EnumType>()>;

template<>
constexpr auto EnumToStringMap<Origin>() -> std::array<const char*, EnumSize<Origin>()> {
	return { {
		{ "Init" },
		{ "Center" },
		{ "LeftTop" },
		{ "Count" }
	} };
}

template<typename EnumType>
std::string EnumToString(EnumType value) 
{
	constexpr auto list = EnumToStringMap<EnumType>();
	return list[static_cast<int>(value)];
}

template<typename EnumType>
EnumType StringToEnum(const string& str)
{
	constexpr auto list = EnumToStringMap<EnumType>();
	int iter{ 0 };
	for (const auto & enumStr : list)
	{
		if (enumStr == str)
			break;
		iter++;
	}

	return static_cast<EnumType>(iter);
}

struct ImageSource
{
	wstring filename{};
	vector<Rectangle> list;

	bool Read(const nlohmann::json& data);
};
