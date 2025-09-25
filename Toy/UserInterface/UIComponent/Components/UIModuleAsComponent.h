#pragma once
#include "../UIComponent.h"

class UIModule;
class UIModuleAsComponent : public UIComponent
{
public:
	~UIModuleAsComponent();
	UIModuleAsComponent();

	static ComponentID GetTypeStatic() { return ComponentID::TextureSwitcher; }
	virtual ComponentID GetTypeID() const noexcept override { return GetTypeStatic(); }
	inline UIModule* GetUIModule() const noexcept { return m_module.get(); }

protected:
	virtual unique_ptr<UIComponent> CreateClone() const;
	virtual bool ImplementBindSourceInfo(TextureResourceBinder*, ITextureController*) noexcept;
	//GetChildren으로 UIModule의 자식들을 연결해 주기 때문에 이 클래스에 Update와 Render 함수가 필요 없다.
	virtual const vector<unique_ptr<UIComponent>>& GetChildren() const noexcept override;

public:
	bool Setup(unique_ptr<UIModule> module) noexcept;

private:
	unique_ptr<UIModule> m_module;
};