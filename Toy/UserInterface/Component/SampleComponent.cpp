#include "pch.h"
#include "SampleComponent.h"
#include "../UILayout.h"
#include "ImageGrid1.h"
#include "ImageGrid3.h"
#include "ImageGrid9.h"
#include "Button.h"
#include "TextArea.h"
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

unique_ptr<UIComponent> CreateSampleTextArea(const UILayout& layout)
{
	map<wstring, wstring> fontFileList;
	fontFileList.insert(make_pair(L"Hangle", L"UI/Font/MaleunGothicS16.spritefont"));
	fontFileList.insert(make_pair(L"English", L"UI/Font/CourierNewBoldS18.spritefont"));
	wstring text = L"<Hangle><Red>테스<br>트, 테스트2</Red>!@#$% </Hangle><English>Test. ^<Blue>&*</Blue>() End</English>";

	return CreateTextArea(layout, text, fontFileList);
}

static ImageSource CreateImageSource(const wstring& texturePath, const vector<Rectangle>& rects) noexcept
{
	return ImageSource{ texturePath, rects };
}

template <typename ImageGridType>
static unique_ptr<UIComponent> CreateSampleButton(const UILayout& layout, 
	const vector<vector<Rectangle>>& sources, const wstring& texturePath)
{
	UILayout gridLayout(layout.GetSize(), Origin::LeftTop);

	vector<unique_ptr<UIComponent>> imageGridList;
	for (const auto& rects : sources)
	{
		ImageSource imgSrc = CreateImageSource(texturePath, rects);
		imageGridList.emplace_back(CreateImageGrid<ImageGridType>(gridLayout, imgSrc));
	}

	return CreateButton(layout, move(imageGridList));
}

unique_ptr<UIComponent> CreateSampleButton3(const UILayout& layout)
{
	vector<vector<Rectangle>> sources{
		{ {10, 82, 22, 48}, {32, 82, 4, 48}, {36, 82, 22, 48} },
		{ {62, 82, 22, 48}, {84, 82, 4, 48}, {88, 82, 22, 48} },
		{ {114, 82, 22, 48}, {136, 82, 4, 48}, {140, 82, 22, 48} } };
	return CreateSampleButton<ImageGrid3>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}

unique_ptr<UIComponent> CreateSampleButton1(const UILayout& layout)
{
	vector<vector<Rectangle>> sources{
		{ {10, 138, 32, 32} },
		{ {46, 138, 32, 32} },
		{ {82, 138, 32, 32} } };
	return CreateSampleButton<ImageGrid1>(layout, sources, L"UI/SampleTexture/Sample_0.png");
}