#include "pch.h"
#include "ImagePartSet.h"
#include "../../Include/IRenderer.h"
#include "UIType.h"
#include "UILayout.h"
#include "ImagePart.h"
#include "../Utility.h"

ImagePartSet::~ImagePartSet() = default;
ImagePartSet::ImagePartSet(const wstring& resPath, const vector<ImageSource>& imgSources)
{
	for (const auto& imgSource : imgSources)
	{
		const wstring& filename = resPath + imgSource.filename;
		ranges::transform(imgSource.list, back_inserter(m_images), [&filename](const auto& source) {
			return make_unique<ImagePart>(filename, source);
			});
	}
}

bool ImagePartSet::LoadResources(ILoadData* load)
{
	for (auto& part : m_images)
		ReturnIfFalse(part->Load(load));
	return true;
}

void ImagePartSet::SetDestination(const UILayout* layout)
{
	const auto& leftSize = m_images[Part::Left]->GetSize();
	const auto& centerSize = m_images[Part::Center]->GetSize();
	const auto& rightSize = m_images[Part::Right]->GetSize();

	XMUINT2 curCenterSize{ centerSize };
	curCenterSize.x = max(0u, static_cast<uint32_t>(layout->GetArea().width) - leftSize.x - rightSize.x);
	m_images[Part::Center]->SetSize(curCenterSize);

	vector<XMUINT2> destination(Part::Count);
	destination[Part::Left] = { 0, 0 };
	destination[Part::Center] = { leftSize.x, 0 };
	destination[Part::Right] = { leftSize.x + curCenterSize.x, 0 };

	vector<XMUINT2> size(Part::Count);
	size[Part::Left] = leftSize;
	size[Part::Center] = curCenterSize;
	size[Part::Right] = rightSize;

	for (auto part : { Left, Center, Right })
	{
		m_images[part]->SetDestination(Rectangle(
			destination[part].x,
			destination[part].y,
			size[part].x, size[part].y));
	}
}

void ImagePartSet::SetPosition(const XMUINT2& position)
{
	for (const auto& part : m_images)
		part->SetPosition(position);
}

bool ImagePartSet::IsHover(int mouseX, int mouseY)
{
	for (const auto& part : m_images)
	{
		auto result = part->IsHover(mouseX, mouseY);
		if (result) return result;
	}

	return false;
}

void ImagePartSet::Render(IRender* render)
{
	for (const auto& part : m_images)
		part->Render(render);
}