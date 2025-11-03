#pragma once
#include "../UIComponent.h"
#include "../UIHelperClass.h"

enum class InputState;
class PatchTextureStd3;
class TextureSwitcher;

class ScrollBar : public UIComponent
{
public:
	ScrollBar();
	~ScrollBar();
	static ComponentID GetTypeStatic() { return ComponentID::ScrollBar; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& o) const noexcept override;
	virtual bool OnWheel(int wheelValue) noexcept override;
	virtual void ProcessIO(SerializerIO& serializer) override;

	void AddScrollChangedCB(function<void(float)> callback) { m_onScrollChangedCB = callback; }
	void RestoreDefault() noexcept;
	bool Setup(const UILayout& layout, unique_ptr<PatchTextureStd3> scrollTrack, unique_ptr<TextureSwitcher> scrollButton);
	bool Setup(unique_ptr<PatchTextureStd3> scrollTrack, unique_ptr<TextureSwitcher> scrollButton);
	bool UpdateScrollView(uint32_t viewArea, uint32_t contentSize) noexcept;
	void SetPositionRatio(float positionRatio) noexcept;
	void SetEnableWheel(bool enable) noexcept;

protected:
	ScrollBar(const ScrollBar& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept;

private:
	void ReloadDatas() noexcept;
	void SetScrollContainerSize(float ratio) noexcept;
	template<typename ReturnType>
	inline ReturnType GetMaxScrollRange() const noexcept;
	void OnPressCB(InputState inputState);

	PatchTextureStd3* m_scrollTrack;
	TextureSwitcher* m_scrollButton;
	BoundedValue m_bounded;
	bool m_isWheelEnabled{ false };
	int m_wheelValue{ 0 };
	int32_t m_pressMousePos{ 0 };
	XMINT2 m_pressContainerPos{};
	function<void(float)> m_onScrollChangedCB;
};