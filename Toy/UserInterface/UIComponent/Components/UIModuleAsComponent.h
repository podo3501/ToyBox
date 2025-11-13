#pragma once
#include "../UIComponent.h"

class UIModul2;
class UIModuleAsComponent : public UIComponent
{
public:
	~UIModuleAsComponent();
	UIModuleAsComponent();

	static ComponentID GetTypeStatic() { return ComponentID::UIModuleAsComponent; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	bool Setup(UIModul2* modul2) noexcept;
	inline UIModul2* GetUIModule() const noexcept { return m_modul2; }

protected:
	virtual unique_ptr<UIComponent> CreateClone() const override;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept override;
	virtual bool ImplementUpdate(const DX::StepTimer& timer) noexcept override;
	virtual void ImplementRender(ITextureRender* render) const override;

private:
	UIModul2* m_modul2{ nullptr };
};