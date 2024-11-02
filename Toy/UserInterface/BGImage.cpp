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
	ifstream file(filename);
	ReturnIfFalse(file.is_open());

	json dataList = json::parse(file);
	for (const auto& [component, data] : dataList.items())
	{
		auto [item, position] = GetComponent(component, data);
		ReturnIfNullptr(item);

		m_renderItems.emplace_back(make_pair(position, move(item)));
	}

	//json data = json::parse(file);
	//const json& imagepartSet = data["ImagePartSet"];

	//ImageSource sources;
	//sources.filename = StringToWString(imagepartSet["Filename"]);
	//	
	//auto& posList = imagepartSet["Position"];
	//for (size_t i{ 0 }; posList.size() != i; i++)
	//	sources.list.emplace_back(posList[i][0], posList[i][1], posList[i][2], posList[i][3]);

	////m_layout = make_unique<UILayout>(layout);

	////m_imagePartSet = make_unique<ImagePartSet>(sources);

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
