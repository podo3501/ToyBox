#include "pch.h"
#include "Dialog.h"
#include "../../Include/IRenderer.h"
#include "../Utility.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePartSet.h"

Dialog::~Dialog() = default;
Dialog::Dialog()
{}

bool Dialog::LoadResources(ILoadData* load)
{ 
	ReturnIfFalse(m_imagePartSet->LoadResources(load));
	ReturnIfFalse(m_imagePartSet->SetDestination(m_layout->GetArea()));

	return true;
}

void Dialog::SetImage(
	const wstring& resPath,
	IRenderer* renderer,
	const ImageSource& sources,
	const UILayout& layout)
{
	m_layout = make_unique<UILayout>(layout);

	m_imagePartSet = make_unique<ImagePartSet>(resPath, sources);

	renderer->AddLoadResource(this);
}

bool Dialog::ChangeArea(const Rectangle& area) noexcept
{
	ReturnIfFalse(m_imagePartSet->SetDestination(area));

	m_layout->Set(move(area));

	return true;
}

void Dialog::Update(const Vector2& resolution) noexcept
{
	m_imagePartSet->SetPosition(m_layout->GetPosition(resolution));
}

void Dialog::Render(IRender* renderer)
{
	m_imagePartSet->Render(renderer);
}

bool Dialog::IsPicking(const Vector2& pos) const noexcept
{
	return m_layout->IsArea(pos);
}

const Rectangle& Dialog::GetArea() const noexcept
{
	return m_layout->GetArea();
}
