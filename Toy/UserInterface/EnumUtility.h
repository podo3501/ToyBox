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
