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
	//GetChildren���� UIModule�� �ڽĵ��� ������ �ֱ� ������ �� Ŭ������ Update�� Render �Լ��� �ʿ� ����.
	virtual const vector<unique_ptr<UIComponent>>& GetChildren() const noexcept override;

public:
	bool Setup(unique_ptr<UIModule> module) noexcept;

private:
	unique_ptr<UIModule> m_module;
};