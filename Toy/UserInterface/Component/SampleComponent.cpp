#include "pch.h"
#include "SampleComponent.h"
#include "../Include/IRenderer.h"
#include "../UILayout.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "ImageGrid9.h"
#include "Button.h"
#include "TextArea.h"
#include "Container.h"
#include "ListArea.h"
#include "ScrollBar.h"
#include "../UIUtility.h"
#include "../../Utility.h"

unique_ptr<ImageGrid1> CreateSampleImageGrid1(const UILayout& layout)
{
	ImageSource grid1Source{ L"UI/SampleTexture/Sample_0.png", { { 10, 10, 64, 64 } } };
	return CreateImageGrid1(layout, grid1Source);
}

unique_ptr<ImageGrid3> CreateSampleImageGrid3(DirectionType dirType, const UILayout& layout)
{
	ImageSource source{};
	source.filename = L"UI/SampleTexture/Sample_0.png";

	switch (dirType)
	{
	case DirectionType::Horizontal: source.list = { { 10, 82, 22, 48 }, {32, 82, 4, 48}, {36, 82, 22, 48} }; break;
	case DirectionType::Vertical: source.list = { { 10, 82, 48, 22 }, {10, 104, 48, 4}, {10, 108, 48, 22} }; break;
	}

	auto imgGrid3 = make_unique<ImageGrid3>();
	return imgGrid3->SetImage(dirType, layout, source) ? move(imgGrid3) : nullptr;
}

unique_ptr<ImageGrid9> CreateSampleImageGrid9(const UILayout& layout)
{
	ImageSource source{
		L"UI/SampleTexture/Sample_0.png", {
			{ 10, 10, 30, 36 }, { 40, 10, 4, 36 }, { 44, 10, 30, 36 },
			{ 10, 46, 30, 2 }, { 40, 46, 4, 2 }, { 44, 46, 30, 2 },
			{ 10, 48, 30, 26 }, { 40, 48, 4, 26 }, { 44, 48, 30, 26 }
		} };
	return CreateImageGrid9(layout, source);
}

unique_ptr<TextArea> CreateSampleTextArea(const UILayout& layout, const wstring& text)
{
	map<wstring, wstring> fontFileList;
	fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
	fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));

	return CreateTextArea(layout, text, fontFileList);
}

static ImageSource CreateImageSource(const wstring& texturePath, const vector<Rectangle>& rects) noexcept
{
	return ImageSource{ texturePath, rects };
}

template <typename ImageGridFunc>
static unique_ptr<Button> CreateSampleButton( const UILayout& layout, const map<InteractState, vector<Rectangle>>& sources,
	const wstring& texturePath, ImageGridFunc createImageGridFunc)
{
	UILayout gridLayout(layout.GetSize(), Origin::LeftTop);
	map<InteractState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& [state, rects] : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects);
		imageGridList.emplace(state, createImageGridFunc(gridLayout, imgSrc));
	}

	return CreateButton(layout, CreateContainer(gridLayout, move(imageGridList), BehaviorMode::Normal));
}

static unique_ptr<Button> CreateSampleButton1(const UILayout& layout,
	const map<InteractState, vector<Rectangle>>& sources, const wstring& texturePath)
{
	return CreateSampleButton(layout, sources, texturePath,
		[](const UILayout& layout, const ImageSource& imgSrc) {
		return CreateImageGrid1(layout, imgSrc);
	});
}

static unique_ptr<Button> CreateSampleButton3(DirectionType dirType, const UILayout& layout,
	const map<InteractState, vector<Rectangle>>& sources, const wstring& texturePath)
{
	return CreateSampleButton(layout, sources, texturePath,
		[dirType](const UILayout& layout, const ImageSource& imgSrc) {
			return CreateImageGrid3(dirType, layout, imgSrc);
		});
}

unique_ptr<Button> CreateSampleButton1(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {10, 138, 32, 32} } },
		{ InteractState::Hover, { {46, 138, 32, 32} } },
		{ InteractState::Pressed, { {82, 138, 32, 32} } } };
	return CreateSampleButton1(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

unique_ptr<Button> CreateSampleButton3(DirectionType dirType, const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{};
	switch (dirType)
	{
	case DirectionType::Horizontal:
		sources = { { InteractState::Normal, { {10, 82, 22, 48}, {32, 82, 4, 48}, {36, 82, 22, 48} } },
			{ InteractState::Hover, { {62, 82, 22, 48}, {84, 82, 4, 48}, {88, 82, 22, 48} } },
			{ InteractState::Pressed, { {114, 82, 22, 48}, {136, 82, 4, 48}, {140, 82, 22, 48} } } }; break;
	case DirectionType::Vertical:
		sources = { { InteractState::Normal, { {10, 82, 48, 22}, {10, 104, 48, 4}, {10, 108, 48, 22} } },
			{ InteractState::Hover, { {62, 82, 48, 22}, {62, 104, 48, 4}, {62, 108, 48, 22} } },
			{ InteractState::Pressed, { {114, 82, 48, 22}, {114, 104, 48, 4}, {114, 108, 48, 22} } } }; break;
	}
	return CreateSampleButton3(dirType, layout, sources, L"UI/SampleTexture/Sample_0.png");
}

static unique_ptr<ListArea> CreateSampleListArea(const UILayout& layout, unique_ptr<Container> container)
{
	ImageSource imgGrid1Source{ L"UI/SampleTexture/Sample_0.png", { { 10, 178, 48, 48 } } }; //리스트 배경 그림
	auto imgGrid1 = CreateImageGrid1({ layout.GetSize(), Origin::LeftTop }, imgGrid1Source);

	UILayout scrollBarLayout({ {16, layout.GetSize().y }, Origin::LeftTop });
	auto scrollBar = CreateSampleScrollBar(DirectionType::Vertical, scrollBarLayout);

	return CreateListArea(layout, move(imgGrid1), move(container), move(scrollBar));
}

static unique_ptr<Container> CreateListContainer(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {118, 138, 32, 32} } },
		{ InteractState::Hover, { {154, 138, 32, 32} } },
		{ InteractState::Pressed, { {190, 138, 32, 32} } } };

	UILayout gridLayout({ { layout.GetSize().x, 30 }, Origin::LeftTop });	//컨테이너 크기는 넓이는 같고, 높이는 30
	map<InteractState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(L"UI/SampleTexture/Sample_0.png", rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid1(gridLayout, imgSrc));
	}

	return CreateContainer(gridLayout, move(imageGridList), BehaviorMode::Normal);
}

unique_ptr<ListArea> CreateSampleListArea1(const UILayout& layout)
{
	auto listArea = CreateSampleListArea(layout, CreateListContainer(layout));
	//auto listAreaPtr = ComponentCast<ListArea*>(listArea.get());
	//if (!MakeSampleListAreaData(listAreaPtr)) return nullptr;
	return listArea;
}

bool MakeSampleListAreaData(IRenderer* renderer, ListArea* listArea)
{
	//글자가 크기에 안 맞으면 안찍힌다. 
	auto protoTextArea = CreateSampleTextArea({ { 130, 30 }, Origin::Center }, L"");
	ReturnIfFalse(renderer->LoadComponent(protoTextArea.get()));

	protoTextArea->Rename("ListTextArea");
	auto prototype = listArea->GetPrototypeContainer();
	auto failed = UIEx(prototype).AttachComponent(move(protoTextArea), { 75, 15 });
	if (failed) return false; //실패하면 Component가 반환된다. attach는 nullptr이 나와야 잘 붙은 것이다.

	const int& itemCount = 7;
	for (auto idx : views::iota(0, itemCount))
	{
		auto container = listArea->PrepareContainer();
		TextArea* textArea = UIEx(container).GetComponent<TextArea*>("ListTextArea_" + to_string(idx));
		textArea->SetText(L"<English><Black>Test " + IntToWString(idx*10) + L"</Black></English>");
	}

	return true;
}

unique_ptr<Container> CreateScrollContainer(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {134, 178, 16, 7}, {134, 185, 16, 2}, {134, 187, 16, 7} } },
		{ InteractState::Hover, { {154, 178, 16, 7}, {154, 185, 16, 2}, {154, 187, 16, 7} } },
		{ InteractState::Pressed, { {174, 178, 16, 7}, {174, 185, 16, 2}, {174, 187, 16, 7} } } };

	UILayout imgLayout(layout.GetSize(), Origin::LeftTop);
	map<InteractState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(L"UI/SampleTexture/Sample_0.png", rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid3(DirectionType::Vertical, imgLayout, imgSrc));
	}
	return CreateContainer(layout, move(imageGridList), BehaviorMode::HoldToKeepPressed);
}

unique_ptr<ScrollBar> CreateSampleScrollBar(DirectionType dirType, const UILayout& layout)
{
	UILayout gridLayout({ layout.GetSize(), Origin::LeftTop });
	ImageSource trackSource{
		L"UI/SampleTexture/Sample_0.png", {
			{ 114, 178, 16, 10 }, { 114, 188, 16, 28 }, { 114, 216, 16, 10 }
		} };
	
	return CreateScrollBar(layout, 
		CreateImageGrid3(dirType, gridLayout, trackSource),
		CreateScrollContainer(gridLayout));
}
