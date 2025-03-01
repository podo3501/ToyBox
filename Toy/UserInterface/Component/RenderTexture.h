#pragma once
#include "../UIComponent.h"

struct IRenderer;
namespace DX { class StepTimer; }

class RenderTexture : public UIComponent
{
public:
	~RenderTexture();
	RenderTexture();

	static ComponentID GetTypeStatic() { return ComponentID::RenderTexture; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent>&& component) noexcept;
	bool ModifyTexture(const XMUINT2& size);

	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }
	inline UIComponent* GetRenderedComponent() const noexcept { return m_component; }
	inline bool IsMouseInArea() const noexcept { return m_mouseInArea; }
	inline void EnableChildMouseEvents(bool enable) noexcept { m_mouseEvents = enable; }
	inline bool OnEnterArea() const noexcept { return m_entered; }
	inline bool OnLeaveArea() const noexcept { return m_left; }

protected:
	RenderTexture(const RenderTexture& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementPostLoaded(ITextureController*) override;
	virtual bool ImplementUpdate(const DX::StepTimer&) noexcept override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	void Release() noexcept;
	void ReloadDatas() noexcept;
	void CheckMouseInArea() noexcept;
	void CheckEnterLeave() noexcept;

	UIComponent* m_component;
	ITextureController* m_texController;

	optional<size_t> m_index;
	UINT64 m_gfxOffset{};

	bool m_mouseEvents{ true };
	bool m_mouseInArea{ false };
	bool m_lastMouseInArea{ false };
	bool m_entered{ false };
	bool m_left{ false };
};

unique_ptr<RenderTexture> CreateRenderTexture(const UILayout& layout, unique_ptr<UIComponent>&& component);
