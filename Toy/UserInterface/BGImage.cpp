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
		case DataType::Name:
			m_name = dataList["Name"];
			break;
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

void BGImage::SetImage(IRenderer* renderer, const string& name, const Vector2 position, const UILayout& layout, const ImageSource& sources)
{
	m_name = name;
	m_position = position;
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

bool BGImage::Update(const Vector2& position) noexcept
{
	const Vector2 pos = m_layout->GetPosition(m_position + position);
	ReturnIfFalse(m_imagePartSet->Update(pos));	//Update보다 SetPosition으로 바꾸는게 더 직관적일듯

	return true;
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

BGImage::BGImage(const BGImage& other)
{
	m_name = other.m_name + "_clone";
	m_position = other.m_position;
	m_layout = make_unique<UILayout>(*other.m_layout.get());
	m_imagePartSet = make_unique<ImagePartSet>(*other.m_imagePartSet.get());
}

unique_ptr<IRenderItem> BGImage::Clone()
{
	return make_unique<BGImage>(*this);
}

void BGImage::SetPosition(const string& name, const Vector2& pos) noexcept
{
	m_position = pos;
}