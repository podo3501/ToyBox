#pragma once
#include "../UIComponent.h"
#include "../UIHelperClass.h"

enum class KeyState;
class PatchTextureStd3;
class TextureSwitcher;
class Container;

class ScrollSlider : public UIComponent
{
public:
	ScrollSlider();
	~ScrollSlider();
	static ComponentID GetTypeStatic() { return ComponentID::ScrollSlider; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	void AddScrollChangedCB(function<void(float)> callback) { m_onScrollChangedCB = callback; }
	bool Setup(const UILayout& layout, 
		unique_ptr<PatchTextureStd3> scrollTrack,
		unique_ptr<TextureSwitcher> scrollButton);
	void SetViewContent(uint32_t viewArea, uint32_t contentSize) noexcept;
	void SetPositionRatio(float positionRatio) noexcept;
	void SetEnableWheel(bool enable) noexcept;

protected:
	ScrollSlider(const ScrollSlider& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size) noexcept;

private:
	void ReloadDatas() noexcept;
	void SetScrollContainerSize(float ratio) noexcept;
	template<typename ReturnType>
	inline ReturnType GetMaxScrollRange() const noexcept;
	void OnPressCB(KeyState keyState);

	PatchTextureStd3* m_scrollTrack;
	TextureSwitcher* m_scrollButton;
	BoundedValue m_bounded;
	bool m_isWheelEnabled{ false };
	int32_t m_pressMousePos{ 0 };
	XMINT2 m_pressContainerPos{};
	function<void(float)> m_onScrollChangedCB;
};

unique_ptr<ScrollSlider> CreateScrollSlider(const UILayout& layout,
	unique_ptr<PatchTextureStd3> scrollTrack,
	unique_ptr<TextureSwitcher> scrollButton);