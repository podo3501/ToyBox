#include "pch.h"
#include "SampleComponent.h"
#include "../Include/IRenderer.h"
#include "../UIComponent.h"
#include "../UILayout.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd1.h"
#include "PatchTexture/PatchTextureStd/PatchTextureStd3.h"
#include "TextArea.h"
#include "ListArea.h"
#include "ScrollBar.h"
#include "ScrollSlider.h"
#include "TextureSwitcher.h"
#include "Utility.h"

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
	UILayout layout{ size, Origin::LeftTop };
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
		[](UILayout& layout, const string& key) { return CreatePatchTextureStd1(layout, key); });
}

map<InteractState, unique_ptr<UIComponent>> GetComponentKeyMap(DirectionType dirType, const XMUINT2& size, const string& bindKey)
{
	return GetComponentKeyMap(size, bindKey,
		[dirType](UILayout& layout, const string& key) { return CreatePatchTextureStd3(layout, dirType, key); });
}

unique_ptr<ScrollSlider> CreateSampleScrollSlider(DirectionType dirType, const UILayout& layout)
{
	UILayout gridLayout({ layout.GetSize(), Origin::LeftTop });
		
	return CreateScrollSlider(layout,
		CreatePatchTextureStd3(gridLayout, dirType, "ScrollTrack3_V"),
		CreateTextureSwitcher(gridLayout, DirTypeToTextureSlice(dirType), GetStateKeyMap("ScrollButton3_V"), BehaviorMode::HoldToKeepPressed));
}

unique_ptr<ScrollBar> CreateSampleScrollBar(const UILayout& layout)
{
	UILayout bgLayout({ layout.GetSize(), Origin::LeftTop });
	const XMUINT2& padding{ 6, 6 };
	UILayout sliderLayout({ layout.GetSize() - padding, Origin::LeftTop });

	return CreateScrollBar(layout,
		CreatePatchTextureStd1(bgLayout, "ListBackground1_Normal"),
		CreateSampleScrollSlider(DirectionType::Vertical, sliderLayout));
}

unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout)
{
	UILayout backImgLayout{ layout.GetSize(), Origin::LeftTop };
	auto listBackImage = CreatePatchTextureStd1(backImgLayout, "ListBackImage1");

	UILayout scrollSliderLayout({ {22, layout.GetSize().y }, Origin::LeftTop });
	auto scrollSlider = CreateSampleScrollSlider(DirectionType::Vertical, scrollSliderLayout);

	/*UILayout scrollBarLayout({ {22, layout.GetSize().y }, Origin::LeftTop });
	auto scrollBar = CreateSampleScrollBar(scrollBarLayout);*/

	UILayout switcherLayout({ { layout.GetSize().x, 32 }, Origin::LeftTop });	//컨테이너 크기는 넓이는 같고, 높이는 32
	auto switcher = CreateTextureSwitcher(switcherLayout, TextureSlice::Nine,
		GetStateKeyMap("ListBackground9"), BehaviorMode::Normal);

	return CreateListArea(layout, move(listBackImage), move(switcher), move(scrollSlider));
}

bool MakeSampleListAreaData(IRenderer* renderer, TextureResourceBinder* rb, ListArea* listArea, int itemCount)
{
	//글자가 크기에 안 맞으면 안찍힌다. 
	vector<wstring> bindKeys{ L"Hangle", L"English" };
	auto protoTextArea = CreateTextArea({ {200, 30}, Origin::LeftTop }, L"", bindKeys);
	ReturnIfFalse(protoTextArea->BindTextureSourceInfo(rb, renderer->GetTextureController()));

	protoTextArea->Rename("ListTextArea");
	auto prototype = listArea->GetPrototypeContainer();
	auto failed = prototype->AttachComponentToCenter(move(protoTextArea), { 0, -6 });
	if (failed) return false; //실패하면 Component가 반환된다. attach는 nullptr이 나와야 잘 붙은 것이다.

	for (auto idx : views::iota(0, itemCount))
	{
		auto container = listArea->PrepareContainer();
		TextArea* textArea = UIEx(container).FindComponent<TextArea*>("ListTextArea_" + to_string(idx));
		textArea->SetText(L"<English><Black>Test " + IntToWString(idx * 10) + L"</Black></English>");
	}

	return true;
}