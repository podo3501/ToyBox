#include "pch.h"
#include "BGImage.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"
#include "JsonHelper.h"

using json = nlohmann::json;

BGImage::~BGImage() = default;
BGImage::BGImage()
{}

bool BGImage::LoadResources(ILoadData* load)
{ 
	return ranges::all_of(m_NimagePartSet, [this, load](const auto& item) {
		const auto& imagePartSet = item.second.get();
		ReturnIfFalse(imagePartSet->LoadResources(load));
		ReturnIfFalse(imagePartSet->SetDestination(m_layout->GetArea()));
		return true;
		});
}

bool BGImage::SetResources(const wstring& filename)
{
	ifstream file(filename);
	ReturnIfFalse(file.is_open());

	json dataList = json::parse(file);
	for (const auto& [key, data] : dataList.items())
	{
		auto dataType = GetType(key);
		if (dataType == DataType::Init) return false;

		switch (dataType)
		{
		case DataType::Layout:
			m_layout = make_unique<UILayout>(data);
			break;
		case DataType::Property:
			auto [imagePartSet, position] = GetProperty<ImagePartSet>(data);
			ReturnIfNullptr(imagePartSet);
			m_NimagePartSet.emplace_back(make_pair(position, move(imagePartSet)));
			break;
		}
	}

	return true; 
}

void BGImage::SetImage(
	IRenderer* renderer,
	const ImageSource& sources,
	const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);

	m_imagePartSet = make_unique<ImagePartSet>(sources);

	renderer->AddLoadResource(this);
}

bool BGImage::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(m_imagePartSet->SetDestination(area));

	m_layout->Set(move(area));

	return true;
}

void BGImage::Update(const Vector2& resolution) noexcept
{
	m_imagePartSet->SetPosition(m_layout->GetPosition(resolution));
}

void BGImage::Update(const Vector2& normalPos, const Vector2& resolution) noexcept
{
	const Vector2 pos = m_layout->GetPosition(resolution) + (resolution * normalPos);
	m_imagePartSet->SetPosition(pos);
}

void BGImage::Render(IRender* renderer)
{
	m_imagePartSet->Render(renderer);
}

bool BGImage::IsPicking(const Vector2& pos) const noexcept
{
	return m_layout->IsArea(pos);
}

const Rectangle& BGImage::GetArea() const noexcept
{
	return m_layout->GetArea();
}
