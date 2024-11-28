#include "pch.h"
#include "ImagePartSet.h"
#include "UIType.h"
#include "ImagePart.h"
#include "../Utility.h"
#include "JsonOperation.h"

using json = nlohmann::json;

ImagePartSet::ImagePartSet() = default;
ImagePartSet::~ImagePartSet() = default;
ImagePartSet::ImagePartSet(const vector<ImageSource>& imgSources)
{
	ranges::for_each(imgSources, [this](const auto& source) { CreateImagePart(source); });
}

bool ImagePartSet::operator==(const ImagePartSet& o) const noexcept
{
	return CompareSeq(m_images, o.m_images);
}

string ImagePartSet::GetType() const { return string(typeid(ImagePartSet).name()); }

ImagePartSet::ImagePartSet(const ImageSource& source)
{
	CreateImagePart(source);
}

ImagePartSet::ImagePartSet(const ImagePartSet& other)
{
	ranges::for_each(other.m_images, [this](const auto& part){
		m_images.emplace_back(make_unique<ImagePart>(*part));
		});
}

void ImagePartSet::CreateImagePart(const ImageSource& imgSource)
{
	const wstring& filename = imgSource.filename;
	if (imgSource.list.empty())	//읽어들이는 부위가 전체 부위와 같을때는 source 정보(이미지의 부분정보)가 없다.
	{
		m_images.emplace_back(make_unique<ImagePart>(filename, Rectangle{}));
		return;
	}

	ranges::transform(imgSource.list, back_inserter(m_images), [&filename](const auto& source) {
		return make_unique<ImagePart>(filename, source);
		});
}

bool ImagePartSet::LoadResources(ILoadData* load)
{
	for (auto& part : m_images)
		ReturnIfFalse(part->Load(load));
	return true;
}

//양 끝단을 빼고 중간길이를 구한다음 총 4점을 리턴한다.
//가로 4점 세로2점이면 3개의 Rectangle을 구할 수 있다.
//중간에 한쪽 방향으로만 늘어나고 옆으로 늘릴때에는 높이, 위아래로 늘릴때는 넓이가 일정하다고 가정한다.
vector<long> GetStretchedSize(long length, long thisEdge, long thatEdge) noexcept
{
	long middle = 0;
	if (length > thisEdge + thatEdge)
		middle = length - (thisEdge + thatEdge);

	return { 0, thisEdge, thisEdge + middle, length };
}

inline XMUINT2 ImagePartSet::GetSize(int vecIdx) const noexcept
{
	return m_images[vecIdx]->GetSize();
}

vector<Rectangle> ImagePartSet::StretchSize(const Rectangle& area) noexcept
{
	vector<Rectangle> destinations{};
	if (m_images.size() == 1)	//이미지가 하나만 있다면 늘리지 않는다.
	{
		destinations.emplace_back(m_images[0]->GetSource());
		return destinations;
	}

	//필요한 데이터는 0, 2, 6 번째 데이터 뿐이다.
	vector<long> xPoints = GetStretchedSize(area.width, GetSize(0).x, GetSize(2).x);
	vector<long> yPoints{};
	if (m_images.size() == 3) yPoints = { 0, static_cast<long>(GetSize(0).y) };
	if (m_images.size() == 9) yPoints = GetStretchedSize(area.height, GetSize(0).y, GetSize(6).y);

	//. . . .	. . . . 
	//. . . .	. . . .
	//. . . .
	//. . . .
	//4x4나 4x2점을 이용해서 Rectangle을 만드는 코드
	for (auto iy = yPoints.begin(); iy != std::prev(yPoints.end()); ++iy)
		for (auto ix = xPoints.begin(); ix != std::prev(xPoints.end()); ++ix)
			destinations.emplace_back(Rectangle(*ix, *iy, *(ix + 1) - *(ix), *(iy + 1) - *(iy)));

	return destinations;
}

bool ImagePartSet::SetDestination(const Rectangle& area) noexcept
{
	vector<Rectangle> dest = StretchSize(area);
	if (dest.empty()) return false;

	ranges::for_each(m_images, [i{ 0 }, &dest](auto& image) mutable { image->SetDestination(dest[i++]); });
	
	return true;
}

bool ImagePartSet::SetPosition(const Vector2& position) noexcept
{
	for (const auto& part : m_images)
		part->SetPosition(position);

	return true;
}

bool ImagePartSet::IsHover(int mouseX, int mouseY) noexcept
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

bool ImagePartSet::IsPicking(const Vector2& pos)  const noexcept
{
	pos;
	return true;
}

const Rectangle& ImagePartSet::GetArea() const noexcept
{
	static Rectangle empty;
	return empty;
}

void ImagePartSet::SerializeIO(JsonOperation* operation)
{
	operation->Process("Images", m_images);
}