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

	void SetImage(
		const wstring& resPath,
		const ImageSource& sources,
		const UILayout& layout);
	bool ChangeArea(Rectangle&& area) noexcept;
	void Update(const Vector2& resolution) noexcept;

private:
	unique_ptr<UILayout> m_layout;
	unique_ptr<ImagePartSet> m_imagePartSet;
};
