#include "pch.h"
#include "SampleComponent.h"
#include "../UILayout.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "ImageGrid9.h"
#include "Button.h"
#include "TextArea.h"
#include "Container.h"
#include "ListArea.h"
#include "../UIUtility.h"

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
	const map<ButtonState, vector<Rectangle>>& sources, const wstring& texturePath)
{
	UILayout gridLayout(layout.GetSize(), Origin::LeftTop);

	map<ButtonState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid<ImageGridType>(gridLayout, imgSrc));
	}

	return CreateButton(layout, CreateContainer(gridLayout, move(imageGridList)));
}

unique_ptr<UIComponent> CreateSampleButton1(const UILayout& layout)
{
	map<ButtonState, vector<Rectangle>> sources{
		{ ButtonState::Normal, { {10, 138, 32, 32} } },
		{ ButtonState::Hover, { {46, 138, 32, 32} } },
		{ ButtonState::Pressed, { {82, 138, 32, 32} } } };
	return CreateSampleButton<ImageGrid1>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

unique_ptr<UIComponent> CreateSampleButton3(const UILayout& layout)
{
	map<ButtonState, vector<Rectangle>> sources{
		{ ButtonState::Normal, { {10, 82, 22, 48}, {32, 82, 4, 48}, {36, 82, 22, 48} } },
		{ ButtonState::Hover, { {62, 82, 22, 48}, {84, 82, 4, 48}, {88, 82, 22, 48} } },
		{ ButtonState::Pressed, { {114, 82, 22, 48}, {136, 82, 4, 48}, {140, 82, 22, 48} } } };
	return CreateSampleButton<ImageGrid3>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

template <typename ImageGridType>
static unique_ptr<UIComponent> CreateSampleListArea(const UILayout& layout,
	const map<ButtonState, vector<Rectangle>>& sources, const wstring& texturePath)
{

	UILayout gridLayout({ { layout.GetSize().x, 30 }, layout.GetOrigin() });	//컨테이너 크기는 넓이는 같고, 높이는 30

	map<ButtonState, unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects.second);
		imageGridList.emplace(rects.first, CreateImageGrid<ImageGridType>(layout, imgSrc));
	}
	auto container = CreateContainer(layout, move(imageGridList));

	ImageSource imgGrid1Source{ L"UI/SampleTexture/Sample_0.png", { { 10, 178, 48, 48 } } }; //리스트 배경 그림
	auto imgGrid1 = CreateImageGrid<ImageGrid1>(layout, imgGrid1Source);

	return CreateListArea(layout, move(imgGrid1), move(container));
}

unique_ptr<UIComponent> CreateSampleListArea1(const UILayout& layout)
{
	map<ButtonState, vector<Rectangle>> sources{
		{ ButtonState::Normal, { {118, 138, 32, 32} } },
		{ ButtonState::Hover, { {154, 138, 32, 32} } },
		{ ButtonState::Pressed, { {190, 138, 32, 32} } } };
	return CreateSampleListArea<ImageGrid1>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}