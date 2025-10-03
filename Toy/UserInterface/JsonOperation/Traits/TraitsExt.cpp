#include "pch.h"
#include "TraitsExt.h"
#include "../../UIComponent/UIType.h"

nlohmann::ordered_json JsonTraitsBase<Origin>::SerializeToJson(const Origin& data) { return EnumToString(data); }
Origin JsonTraitsBase<Origin>::DeserializeFromJson(const nlohmann::json& dataJ)
{
	return json_detail::CreateAndFill<Origin>([&dataJ](Origin& data) {
		data = *StringToEnum<Origin>(dataJ); });
}
