#pragma once
#include "../Include/IRenderItem.h"
class UILayout;
class ImagePart;
struct ImageSource;
struct ILoadData;
struct IRender;

class ImagePartSet : public IRenderItem
{
public:
	ImagePartSet();
	~ImagePartSet();
	ImagePartSet(const ImagePartSet& other);
	ImagePartSet(const vector<ImageSource>& imgSources);
	ImagePartSet(const ImageSource& source);

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept override;
	virtual void Render(IRender* render) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept override;

	bool SetDestination(const Rectangle& area) noexcept;
	//void SetPosition(const Vector2& position) noexcept;
	bool IsHover(int mouseX, int mouseY) noexcept;
	

private:
	void CreateImagePart(const ImageSource& imgSource);
	vector<Rectangle> StretchSize(const Rectangle& area) noexcept;
	inline XMUINT2 GetSize(int vecIdx) const noexcept;

	vector<unique_ptr<ImagePart>> m_images;
};
