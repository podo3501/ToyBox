#include "pch.h"
#include "BGImage.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"

BGImage::~BGImage() = default;
BGImage::BGImage()
{}

bool BGImage::LoadResources(ILoadData* load)
{ 
	ReturnIfFalse(m_imagePartSet->LoadResources(load));
	ReturnIfFalse(m_imagePartSet->SetDestination(m_layout->GetArea()));

	return true;
}

void BGImage::SetImage(
	const wstring& resPath,
	IRenderer* renderer,
	const ImageSource& sources,
	const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);

	m_imagePartSet = make_unique<ImagePartSet>(resPath, sources);

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
