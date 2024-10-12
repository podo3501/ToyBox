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
	ImagePartSet(const wstring& resPath, const vector<ImageSource>& imgSources);
	ImagePartSet(const wstring& resPath, const ImageSource& source);

	bool LoadResources(ILoadData* load);
	bool SetDestination(const Rectangle& area);
	void SetPosition(const XMUINT2& position);
	bool IsHover(int mouseX, int mouseY);
	void Render(IRender* render);

private:
	void CreateImagePart(const wstring& resPath, const ImageSource& imgSource);
	vector<Rectangle> StretchSize(const Rectangle& area) noexcept;
	inline XMUINT2 GetSize(int vecIdx) const noexcept;

	vector<unique_ptr<ImagePart>> m_images;
};
