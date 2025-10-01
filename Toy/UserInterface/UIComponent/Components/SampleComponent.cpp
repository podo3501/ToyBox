#include "pch.h"
#include "SampleComponent.h"
#include "IRenderer.h"
#include "../UIComponent.h"
#include "../UILayout.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "TextArea.h"
#include "ListArea.h"
#include "ScrollBar.h"
#include "TextureSwitcher.h"
#include "Utils/StringUtil.h"

map<InteractState, string> GetStateKeyMap(const string& prefix) noexcept
{
	return {
		{ InteractState::Normal, prefix + "_" + EnumToString(InteractState::Normal) },
		{ InteractState::Hovered, prefix + "_" + EnumToString(InteractState::Hovered) },
		{ InteractState::Pressed, prefix + "_" + EnumToString(InteractState::Pressed) } };
}

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

unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout, DirectionType dirType)
{
	UILayout gridLayout{ layout.GetSize() };
		
	return CreateComponent<ScrollBar>(layout,
		CreateComponent<PatchTextureStd3>(gridLayout, dirType, "ScrollTrack3_V"),
		CreateComponent<TextureSwitcher>(gridLayout, DirTypeToTextureSlice(dirType), GetStateKeyMap("ScrollButton3_V"), BehaviorMode::HoldToKeepPressed));
}

unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout)
{
	return CreateComponent<ListArea>(layout,
		move(CreateComponent<PatchTextureStd1>(UILayout{ layout.GetSize() }, "ListBackImage1")),
		move(CreateComponent<TextureSwitcher>(TextureSlice::Nine, GetStateKeyMap("ListBackground9"), BehaviorMode::Normal)),
		move(CreateSampleScrollBar({}, DirectionType::Vertical)));
}

bool MakeSampleListAreaData(IRenderer* renderer, TextureResourceBinder* rb, ListArea* listArea, int itemCount)
{
	ZoneScopedN("MakeSampleListAreaData");

	//글자가 크기에 안 맞으면 안찍힌다. 
	vector<wstring> bindKeys{ L"Hangle", L"English" };
	auto protoTextArea = CreateComponent<TextArea>(UILayout{ {200, 30}, Origin::LeftTop }, L"", bindKeys);
	ReturnIfFalse(protoTextArea->BindTextureSourceInfo(rb, renderer->GetTextureController()));
	
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