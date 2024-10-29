#pragma once
#include "../Include/IRenderItem.h"

struct IRenderer;
struct ILoadData;

struct ImageSource;
class ImagePartSet;
class UILayout;

enum class Origin;

class Dialog : public IRenderItem
{
public:
	Dialog();
	virtual ~Dialog();

	virtual bool LoadResources(ILoadData* load) override;
	virtual void Render(IRender* renderer) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept;

	void SetImage(
		const wstring& resPath,
		IRenderer* renderer,
		const ImageSource& sources,
		const UILayout& layout);
	bool ChangeArea(const Rectangle& area) noexcept;
	void Update(const Vector2& resolution) noexcept;
	void Update(const Vector2& normalPos, const Vector2& resolution) noexcept;

private:
	unique_ptr<UILayout> m_layout;
	unique_ptr<ImagePartSet> m_imagePartSet;
};
