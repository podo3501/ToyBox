#pragma once

class UILayout;
class ImagePart;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet
{
public:
	ImagePartSet() = delete;
	~ImagePartSet();
	ImagePartSet(const vector<ImageSource>& imgSources);
	ImagePartSet(const ImageSource& source);

	bool LoadResources(ILoadData* load);
	bool SetDestination(const Rectangle& area) noexcept;
	void SetPosition(const Vector2& position) noexcept;
	bool IsHover(int mouseX, int mouseY) noexcept;
	void Render(IRender* render);

private:
	void CreateImagePart(const ImageSource& imgSource);
	vector<Rectangle> StretchSize(const Rectangle& area) noexcept;
	inline XMUINT2 GetSize(int vecIdx) const noexcept;

	vector<unique_ptr<ImagePart>> m_images;
};
