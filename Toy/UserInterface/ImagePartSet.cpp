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
	ranges::for_each(imgSources, [this, &resPath](const auto& source) { CreateImagePart(resPath, source); });
}

ImagePartSet::ImagePartSet(const wstring& resPath, const ImageSource& source)
{
	CreateImagePart(resPath, source);
}

void ImagePartSet::CreateImagePart(const wstring& resPath, const ImageSource& imgSource)
{
	const wstring& fullFilename = resPath + imgSource.filename;
	ranges::transform(imgSource.list, back_inserter(m_images), [&fullFilename](const auto& source) {
		return make_unique<ImagePart>(fullFilename, source);
		});
}

bool ImagePartSet::LoadResources(ILoadData* load)
{
	for (auto& part : m_images)
		ReturnIfFalse(part->Load(load));
	return true;
}

//�� ������ ���� �߰����̸� ���Ѵ��� �� 4���� �����Ѵ�.
//���� 4�� ����2���̸� Rectangle�� ���� �� �ִ�.
//�߰��� ���� �������θ� �þ�� ������ �ø������� ����, ���Ʒ��� �ø����� ���̰� �����ϴٰ� �����Ѵ�.
vector<long> GetStretchedSize(long length, long thisEdge, long thatEdge) noexcept
{
	long middle = 0;
	if (length > thisEdge + thatEdge)
		middle = length - (thisEdge + thatEdge);

	return vector<long>{ 0, thisEdge, thisEdge + middle, length };
}

vector<Rectangle> StretchSize(const Rectangle& area, const vector<XMUINT2>& sizes) noexcept
{
	vector<long> xPoints = GetStretchedSize(area.width, sizes[0].x, sizes[2].x);

	vector<long> yPoints;
	if (sizes.size() == 3)
		yPoints = { 0, static_cast<long>(sizes[0].y) };
	else if (sizes.size() == 9)
		yPoints = GetStretchedSize(area.height, sizes[0].y, sizes[6].y);
	else return {};

	//4, 2�� 4, 4���� �̿��ؼ� Rectangle�� ����� �ڵ�
	vector<Rectangle> destinations;
	for (auto ix = xPoints.begin(); ix != std::prev(xPoints.end()); ++ix)
		for (auto iy = yPoints.begin(); iy != std::prev(yPoints.end()); ++iy)
			destinations.emplace_back(Rectangle(*ix, *iy, *(ix + 1) - *(ix), *(iy + 1) - *(iy)));

	return destinations;
}

bool ImagePartSet::SetDestination(const Rectangle& area)
{
	vector<XMUINT2> sizes;
	ranges::transform(m_images, back_inserter(sizes), [](const auto& part) { return part->GetSize(); });
	vector<Rectangle> destinations = StretchSize(area, sizes);
	if (destinations.empty()) return false;

	int idx = 0;
	for (auto& image : m_images)
		image->SetDestination(destinations[idx++]);

	return true;
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