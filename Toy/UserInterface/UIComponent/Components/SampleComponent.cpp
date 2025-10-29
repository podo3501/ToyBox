#include "pch.h"
#include "SampleComponent.h"
#include "IRenderer.h"
#include "../UIUtility.h"
#include "../UIComponent.h"
#include "../UILayout.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "TextArea.h"
#include "ListArea.h"
#include "ScrollBar.h"
#include "TextureSwitcher.h"
#include "Shared/Utils/StringExt.h"

static map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(
	const XMUINT2& size, const string& bindKey,
	function<unique_ptr<UIComponent>(UILayout&, const string&)> CreatePatchTextureFn)
{
	UILayout layout{ size };
	map<InteractState, unique_ptr<UIComponent>> components;

	const auto& stateKeys = GetStateKeyMap(bindKey);
	for (const auto& pair : stateKeys)
	{
		auto component = CreatePatchTextureFn(layout, pair.second);
		if (!component) return {}; // 실패 시 빈 맵 반환

		components.emplace(pair.first, move(component));
	}
	return components;
}

map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(const XMUINT2& size, const string& bindKey)
{
	return GetComponentKeyMap(size, bindKey,
		[](UILayout& layout, const string& key) { return CreateComponent<PatchTextureStd1>(layout, key); });
}

map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(DirectionType dirType, const XMUINT2& size, const string& bindKey)
{
	return GetComponentKeyMap(size, bindKey,
		[dirType](UILayout& layout, const string& key) { return CreateComponent<PatchTextureStd3>(layout, dirType, key); });
}

unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout, DirectionType dirType,
	const string& trackKey, const string& buttonKey)
{
	UILayout gridLayout{ layout.GetSize() };
		
	return CreateComponent<ScrollBar>(layout,
		CreateComponent<PatchTextureStd3>(gridLayout, dirType, trackKey),
		CreateComponent<TextureSwitcher>(gridLayout, DirTypeToTextureSlice(dirType), GetStateKeyMap(buttonKey), BehaviorMode::HoldToKeepPressed));
}

unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout,
	const string& backImageKey, const string& switcherKey, const string& scrollTrackKey, const string& scrollButtonKey)
{
	return CreateComponent<ListArea>(layout,
		move(CreateComponent<PatchTextureStd1>(UILayout{ layout.GetSize() }, backImageKey)),
		move(CreateComponent<TextureSwitcher>(TextureSlice::Nine, GetStateKeyMap(switcherKey), BehaviorMode::Normal)),
		move(CreateSampleScrollBar({}, DirectionType::Vertical, scrollTrackKey, scrollButtonKey)));
}

bool MakeSampleListAreaData(ITextureController* texController, TextureResourceBinder* rb, ListArea* listArea, int itemCount)
{
	ZoneScopedN("MakeSampleListAreaData");

	//글자가 크기에 안 맞으면 안찍힌다. 
	vector<wstring> bindKeys{ L"Hangle", L"English" };
	auto protoTextArea = CreateComponent<TextArea>(UILayout{ {200, 30}, Origin::LeftTop }, L"", bindKeys);
	ReturnIfFalse(protoTextArea->BindTextureSourceInfo(rb, texController));
	
	UIEx(protoTextArea).Rename("ListTextArea");
	auto prototype = listArea->GetPrototypeContainer();
	auto failed = prototype->AttachComponentToCenter(move(protoTextArea), { 0, -6 });
	if (failed) return false; //실패하면 Component가 반환된다. attach는 nullptr이 나와야 잘 붙은 것이다.

	for (auto idx : views::iota(0, itemCount))
	{
		auto container = listArea->PrepareContainer();
		TextArea* textArea = UIEx(container).FindComponent<TextArea*>("ListTextArea");
		textArea->SetText(L"<English><Black>Test " + IntToWString(idx * 10) + L"</Black></English>");
	}

	return true;
}