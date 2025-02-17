#pragma once
#include "../UIComponent.h"

struct IRenderer;

class RenderTexture : public UIComponent
{
public:
	~RenderTexture();
	RenderTexture();

	static ComponentID GetTypeStatic() { return ComponentID::RenderTexture; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void SerializeIO(JsonOperation& operation) override;

	bool Setup(const UILayout& layout, UIComponent* component);
	bool CreateTexture(IRenderer* renderer, const XMUINT2& size, UIComponent* component);
	bool ModifyTexture(const XMUINT2& size);
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }

protected:
	RenderTexture(const RenderTexture& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementPostLoaded(ITextureController*) override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	void Release() noexcept;
	void ReloadDatas() noexcept;

	UIComponent* m_component;
	ITextureController* m_texController;

	size_t m_index{ 0 };
	UINT64 m_gfxOffset{};
};

unique_ptr<RenderTexture> CreateRenderTexture(const UILayout& layout, UIComponent* component);
