#pragma once

class UILayout;
class ImagePart;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet
{
	//��ư�� 3���� �������� �ִ�. ��ư ����� �ø��� �߰��� �þ��.
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
