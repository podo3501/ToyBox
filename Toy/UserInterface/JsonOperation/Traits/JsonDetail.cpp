#include "pch.h"
#include "JsonDetail.hpp"
#include "../../../Utility.h"

namespace json_detail
{
	string KeyConverter<wstring>::ToKey(const wstring& key) { return WStringToString(key); }
	wstring KeyConverter<wstring>::FromKey(const string& key) { return StringToWString(key); }
}