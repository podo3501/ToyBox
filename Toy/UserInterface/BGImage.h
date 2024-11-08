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
	BGImage(const BGImage& other);

	virtual bool SetResources(const wstring& filename) override;
	virtual bool LoadResources(ILoadData* load) override;
	virtual bool Update(const Vector2& position) noexcept;
	virtual void Render(IRender* renderer) override;
	virtual bool IsPicking(const Vector2& pos)  const noexcept override;
	virtual const Rectangle& GetArea() const noexcept;
	virtual IRenderItem* GetSelected() const noexcept { return nullptr; }
	virtual unique_ptr<IRenderItem> Clone() override;
	virtual void SetPosition(const string& name, const Vector2& pos) noexcept override;
	virtual bool ChangeArea(const Rectangle& area) noexcept override;

	void SetImage(IRenderer* renderer, const string& name, const Vector2 position, const UILayout& layout, const ImageSource& sources);
	
	
private:
	string m_name{};
	Vector2 m_position{};
	unique_ptr<UILayout> m_layout;
	unique_ptr<ImagePartSet> m_imagePartSet;
};
