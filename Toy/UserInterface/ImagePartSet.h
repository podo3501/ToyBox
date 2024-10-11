#pragma once

class UILayout;
class ImagePart;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet
{
	enum ImageCount
	{
		Zero = 0,
		One = 1,
		Three = 3,
		Nine = 9
	};

	enum Part	//버튼은 3개로 나누어져 있다. 버튼 사이즈를 늘리면 중간만 늘어난다.
	{
		Left = 0,
		Center = 1,
		Right = 2,
		Count = 3,
	};

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

	vector<unique_ptr<ImagePart>> m_images;
};
