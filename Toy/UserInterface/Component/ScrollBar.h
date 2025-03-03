#pragma once
#include "../UIComponent.h"

enum class KeyState;
class ImageGrid3;
class Container;

class ScrollBar : public UIComponent
{
public:
	ScrollBar();
	~ScrollBar();
	static ComponentID GetTypeStatic() { return ComponentID::ScrollBar; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	void AddScrollChangedCB(function<void(float)> callback) { m_onScrollChangedCB = callback; }
	bool Setup(const UILayout& layout, unique_ptr<UIComponent> scrollTrack, unique_ptr<UIComponent> scrollButton);
	void SetViewContentRatio(float contentRatio) noexcept;
	void SetViewContent(uint32_t viewArea, uint32_t contentSize) noexcept;
	void SetPositionRatio(float positionRatio) noexcept;

protected:
	ScrollBar(const ScrollBar& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;

private:
	void ReloadDatas() noexcept;
	template<typename ReturnType>
	inline ReturnType GetMaxScrollRange() const noexcept;
	void OnPressCB(KeyState keyState);

	ImageGrid3* m_scrollTrack;
	Container* m_scrollContainer;
	function<void(float)> m_onScrollChangedCB;
};

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout,
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollButton);