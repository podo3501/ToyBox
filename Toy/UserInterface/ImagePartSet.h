#pragma once

class UILayout;
class ImagePart;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet
{
	//버튼은 3개로 나누어져 있다. 버튼 사이즈를 늘리면 중간만 늘어난다.
	enum Part
	{
		Left = 0,
		Center = 1,
		Right = 2,
		Count = 3,
	};

public:
	ImagePartSet() = delete;
	ImagePartSet(const wstring& resPath, const vector<ImageSource>& imgSources);
	~ImagePartSet();

	bool LoadResources(ILoadData* load);
	void SetDestination(const UILayout* layout);
	void SetPosition(const XMUINT2& position);
	bool IsHover(int mouseX, int mouseY);
	void Render(IRender* render);

private:
	vector<unique_ptr<ImagePart>> m_images;
};
