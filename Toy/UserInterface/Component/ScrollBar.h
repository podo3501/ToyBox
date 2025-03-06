#pragma once
#include "../UIComponent.h"
#include "../UIHelperClass.h"

enum class KeyState;
class ImageGrid1;
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
	virtual bool ChangeSize(const XMUINT2& size) noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	void AddScrollChangedCB(function<void(float)> callback) { m_onScrollChangedCB = callback; }
	bool Setup(const UILayout& layout, 
		unique_ptr<UIComponent> scrollBackground,
		unique_ptr<UIComponent> scrollTrack, 
		unique_ptr<UIComponent> scrollButton);
	void SetViewContent(uint32_t viewArea, uint32_t contentSize) noexcept;
	void SetPositionRatio(float positionRatio) noexcept;
	void SetEnableWheel(bool enable) noexcept;

protected:
	ScrollBar(const ScrollBar& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;

private:
	void ReloadDatas() noexcept;
	void SetScrollContainerSize(float ratio) noexcept;
	template<typename ReturnType>
	inline ReturnType GetMaxScrollRange() const noexcept;
	void OnPressCB(KeyState keyState);

	ImageGrid1* m_scrollBackground;
	ImageGrid3* m_scrollTrack;
	Container* m_scrollContainer;
	BoundedValue m_bounded;
	bool m_isWheelEnabled{ false };
	int32_t m_pressMousePos{ 0 };
	XMINT2 m_pressContainerPos{};
	function<void(float)> m_onScrollChangedCB;
};

unique_ptr<ScrollBar> CreateScrollBar(const UILayout& layout,
	unique_ptr<UIComponent> scrollBackground,
	unique_ptr<UIComponent> scrollTrack,
	unique_ptr<UIComponent> scrollButton);