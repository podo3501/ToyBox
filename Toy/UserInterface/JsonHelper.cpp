#include "pch.h"
#include "JsonHelper.h"
#include "../Include/IRenderer.h"
#include "../Utility.h"
#include "Dialog.h"
#include "BGImage.h"

using json = nlohmann::json;

unique_ptr<IRenderItem> CreateComponent(const string& component)
{
	if (component == "Dialog")
		return move(make_unique<Dialog>());
	if (component == "BGImage")
		return move(make_unique<BGImage>());

	return nullptr;
}

tuple<unique_ptr<IRenderItem>, Vector2> GetComponent(const string& component, const json& data)
{
	if (component == "Dialog" || component == "BGImage")
	{
		wstring compFilename{ StringToWString(data["Filename"]) };
		const auto& pos = data["Position"];
		Vector2 position{ pos["x"], pos["y"] };

		unique_ptr<IRenderItem> item = CreateComponent(component);
		if (item->SetResources(compFilename) == false)
			return make_tuple(nullptr, position);

		return make_tuple(move(item), position);
	}

	return make_tuple(nullptr, Vector2{});
}