#include "pch.h"
#include "JsonDetail.hpp"
#include "../../../Utility.h"
#include "../../UIComponent/UIType.h"

namespace json_detail
{
	string KeyConverter<wstring>::ToKey(const wstring& key) { return WStringToString(key); }
	wstring KeyConverter<wstring>::FromKey(const string& key) { return StringToWString(key); }

	string KeyConverter<int>::ToKey(const int& key) { return to_string(key); }
	int KeyConverter<int>::FromKey(const string& key) { return stoi(key); }

	string KeyConverter<InteractState>::ToKey(InteractState state) { return EnumToString(state); }
	InteractState KeyConverter<InteractState>::FromKey(const string& key) { return *StringToEnum<InteractState>(key); }
}