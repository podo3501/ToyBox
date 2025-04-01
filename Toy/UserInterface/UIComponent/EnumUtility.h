#pragma once

template<typename EnumType>
constexpr std::size_t EnumSize();

template<typename EnumType>
constexpr auto EnumToStringMap() -> std::array<const char*, EnumSize<EnumType>()>;

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
	for (const auto& enumStr : list)
	{
		if (enumStr == str)
			break;
		iter++;
	}

	return static_cast<EnumType>(iter);
}

template<typename EnumType>
vector<string> EnumToList()
{
	constexpr auto list = EnumToStringMap<EnumType>();

	std::vector<std::string> result;
	result.reserve(EnumSize<EnumType>());

	for (const auto& enumStr : list)
		result.emplace_back(enumStr);

	return result;
}

template <typename EnumType>
constexpr auto EtoV(EnumType enumerator) noexcept	//EnumToValue 이름이 길어서 줄인다.
{
	return static_cast<std::underlying_type_t<EnumType>>(enumerator);
}
