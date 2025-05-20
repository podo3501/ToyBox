#include "pch.h"
#include "BasicTypes.h"

/////////////////////////////////////////////////////////////////

nlohmann::ordered_json JsonTraits<wstring>::SerializeToJson(wstring& data) { return WStringToString(data); }
wstring JsonTraits<wstring>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<wstring>([&dataJ](wstring& data) {
		data = StringToWString(dataJ); });
}
