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

unique_ptr<UIComponent> CreateSampleImageGrid1(const UILayout& layout)
{
	ImageSource grid1Source{ L"UI/SampleTexture/Sample_0.png", { { 10, 10, 64, 64 } } };

	return CreateImageGrid<ImageGrid1>(layout, grid1Source);
}

unique_ptr<UIComponent> CreateSampleImageGrid3(const UILayout& layout)
{
	ImageSource source{
		L"UI/SampleTexture/Sample_0.png", {
			{ 10, 82, 22, 48 }, {32, 82, 4, 48}, {36, 82, 22, 48}
		} };
	return CreateImageGrid<ImageGrid3>(layout, source);
}

unique_ptr<UIComponent> CreateSampleImageGrid9(const UILayout& layout)
{
	ImageSource source{
		L"UI/SampleTexture/Sample_0.png", {
			{ 10, 10, 30, 36 }, { 40, 10, 4, 36 }, { 44, 10, 30, 36 },
			{ 10, 46, 30, 2 }, { 40, 46, 4, 2 }, { 44, 46, 30, 2 },
			{ 10, 48, 30, 26 }, { 40, 48, 4, 26 }, { 44, 48, 30, 26 }
		} };
	return CreateImageGrid<ImageGrid9>(layout, source);
}

unique_ptr<UIComponent> CreateSampleTextArea(const UILayout& layout, const wstring& text)
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

template <typename ImageGridType>
static unique_ptr<UIComponent> CreateSampleButton(const UILayout& layout,
	const map<InteractState, vector<Rectangle>>& sources, const wstring& texturePath)
{
	UILayout gridLayout(layout.GetSize(), Origin::LeftTop);

	map<InteractState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid<ImageGridType>(gridLayout, imgSrc));
	}

	return CreateButton(layout, CreateContainer(gridLayout, move(imageGridList)));
}

unique_ptr<UIComponent> CreateSampleButton1(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {10, 138, 32, 32} } },
		{ InteractState::Hover, { {46, 138, 32, 32} } },
		{ InteractState::Pressed, { {82, 138, 32, 32} } } };
	return CreateSampleButton<ImageGrid1>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

unique_ptr<UIComponent> CreateSampleButton3(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {10, 82, 22, 48}, {32, 82, 4, 48}, {36, 82, 22, 48} } },
		{ InteractState::Hover, { {62, 82, 22, 48}, {84, 82, 4, 48}, {88, 82, 22, 48} } },
		{ InteractState::Pressed, { {114, 82, 22, 48}, {136, 82, 4, 48}, {140, 82, 22, 48} } } };
	return CreateSampleButton<ImageGrid3>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

template <typename ImageGridType>
static unique_ptr<UIComponent> CreateSampleListArea(const UILayout& layout,
	const map<InteractState, vector<Rectangle>>& sources, const wstring& texturePath)
{
	UILayout gridLayout({ { layout.GetSize().x, 30 }, Origin::LeftTop });	//컨테이너 크기는 넓이는 같고, 높이는 30

	map<InteractState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid<ImageGridType>(gridLayout, imgSrc));
	}
	auto container = CreateContainer(gridLayout, move(imageGridList));

	ImageSource imgGrid1Source{ L"UI/SampleTexture/Sample_0.png", { { 10, 178, 48, 48 } } }; //리스트 배경 그림
	auto imgGrid1 = CreateImageGrid<ImageGrid1>({ layout.GetSize(), Origin::LeftTop }, imgGrid1Source);

	return CreateListArea(layout, move(imgGrid1), move(container));
}

unique_ptr<UIComponent> CreateSampleListArea1(const UILayout& layout)
{
	map<InteractState, vector<Rectangle>> sources{
		{ InteractState::Normal, { {118, 138, 32, 32} } },
		{ InteractState::Hover, { {154, 138, 32, 32} } },
		{ InteractState::Pressed, { {190, 138, 32, 32} } } };

	auto listArea = CreateSampleListArea<ImageGrid1>(layout, sources, L"UI/SampleTexture/Sample_0.png");
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

unique_ptr<UIComponent> CreateSampleScrollBar(const UILayout& layout)
{
	UILayout gridLayout({ layout.GetSize(), Origin::LeftTop });
	ImageSource trackSource{
		L"UI/SampleTexture/Sample_0.png", {
			{ 114, 178, 16, 10 }, { 114, 188, 16, 28 }, { 114, 216, 16, 10 }
		} };
	auto scrollTrack = CreateImageGrid<ImageGrid3>(gridLayout, trackSource);

	map<InteractState, vector<Rectangle>> btnSources{
		{ InteractState::Normal, { {134, 178, 7, 16}, {141, 178, 2, 16}, {143, 178, 7, 16} } },
		{ InteractState::Hover, { {154, 178, 7, 16}, {161, 178, 2, 16}, {163, 178, 7, 16} } },
		{ InteractState::Pressed, { {174, 178, 7, 16}, {181, 178, 2, 16}, {183, 178, 7, 16} } } };
	auto scrollButton = CreateSampleButton<ImageGrid3>(gridLayout, btnSources, L"UI/SampleTexture/Sample_0.png");

	return CreateScrollBar(layout, move(scrollTrack), move(scrollButton));
}
