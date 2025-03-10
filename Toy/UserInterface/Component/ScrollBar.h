#pragma once
#include "../UIComponent.h"

class ImageGrid1;
class ScrollSlider;

class ScrollBar : public UIComponent
{
public:
	ScrollBar();
	~ScrollBar();
	static ComponentID GetTypeStatic() { return ComponentID::ScrollBar; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent> scrollBackground, 
		unique_ptr<UIComponent> scrollSlider) noexcept;
	void SerializeIO(JsonOperation& operation);
	bool UpdateScrollView(uint32_t viewArea, uint32_t contentSize)  noexcept;
	inline ScrollSlider* GetScrollSlider() const noexcept { return m_scrollSlider; }

protected:
	ScrollBar(const ScrollBar& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void ReloadDatas() noexcept;
	inline bool ChangeSliderSize(const XMUINT2& size) noexcept;
	bool ToolUpdateScrollView() noexcept;

	ImageGrid1* m_scrollBackground;
	ScrollSlider* m_scrollSlider;
	uint32_t m_scrollPadding{ 0 };
};

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout, unique_ptr<UIComponent> scrollBackground,
	unique_ptr<UIComponent> scrollSlider);
