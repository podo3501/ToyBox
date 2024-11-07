#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;

class BGImage : public IRenderItem
{
public:
	virtual ~BGImage();
	BGImage();
	BGImage(const UILayout* layout, const ImagePartSet* imagePartSet, const Vector2& position);

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept;
	virtual void Render(IRender* renderer) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept;

	void SetImage(
		IRenderer* renderer,
		const ImageSource& sources,
		const UILayout& layout);
	bool ChangeArea(const Rectangle& area) noexcept;
	unique_ptr<IRenderItem> Clone();
	void SetPosition(const Vector2& pos) noexcept;
	//void Update(const Vector2& normalPos, const Vector2& resolution) noexcept;
	
private:
	unique_ptr<UILayout> m_layout;
	unique_ptr<ImagePartSet> m_imagePartSet;
	Vector2 m_position{};
	//vector<pair<Vector2, unique_ptr<ImagePartSet>>> m_NimagePartSet;
	//vector<pair<Vector2, unique_ptr<IRenderItem>>> m_renderItems;
};
