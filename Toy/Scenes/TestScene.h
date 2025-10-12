#pragma once
#include "Scene.h"

class UIComponent;
class TestScene : public Scene
{
public:
	TestScene(const string& name, IRenderer* renderer);
	
	virtual bool Enter() override;
	virtual bool Leave() override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	bool LoadResources();
	bool AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept;

	unique_ptr<UIModule> m_uiModule;
};