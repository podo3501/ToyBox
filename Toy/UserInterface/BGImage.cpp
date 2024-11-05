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
	ReturnIfFalse(m_imagePartSet->LoadResources(load));
	ReturnIfFalse(m_imagePartSet->SetDestination(m_layout->GetArea()));

	return true;
}

bool BGImage::SetResources(const wstring& filename)
{
	const json& dataList = LoadUIFile(filename);
	if (dataList.is_null())
		return false;

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
			tie(m_imagePartSet, m_position) = GetProperty<ImagePartSet>(data);
			ReturnIfNullptr(m_imagePartSet);
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

	m_layout->Set(area);

	return true;
}

void BGImage::Update(const Vector2& resolution) noexcept
{
	const Vector2 pos = m_layout->GetPosition(resolution) + (resolution * m_position);
	m_imagePartSet->SetPosition(pos);

	//m_imagePartSet->SetPosition(m_layout->GetPosition(resolution));
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
