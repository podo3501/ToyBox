#pragma once
#include "../UIComponent.h"

namespace DX { class StepTimer; }

class RenderTexture : public UIComponent
{
public:
	~RenderTexture();
	RenderTexture();

	static ComponentID GetTypeStatic() { return ComponentID::RenderTexture; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	virtual bool operator==(const UIComponent& rhs) const noexcept override;
	virtual void ProcessIO(SerializerIO& serializer) override;

	bool Setup(const UILayout& layout, unique_ptr<UIComponent> component) noexcept;
	bool Setup(unique_ptr<UIComponent> component) noexcept;
	inline UINT64 GetGraphicMemoryOffset() const noexcept { return m_gfxOffset; }
	inline UIComponent* GetRenderedComponent() const noexcept { return m_component; }

protected:
	RenderTexture(const RenderTexture& other);
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool BindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual void PositionUpdated() noexcept override;
	virtual bool ImplementChangeSize(const XMUINT2& size, bool isForce) noexcept override;
	virtual void Render(ITextureRender* render) const override;

private:
	void AddRef() const noexcept;
	void Release() noexcept;
	void ReloadDatas() noexcept;

	UIComponent* m_component;
	ITextureController* m_texController;

	optional<size_t> m_index;
	UINT64 m_gfxOffset{};
};
