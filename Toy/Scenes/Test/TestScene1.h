#pragma once
#include "../Scene.h"

class UIComponent;
class UIModul2;
class TestScene1 : public Scene
{
public:
	~TestScene1();
	TestScene1(IRenderer* renderer);
	static SceneID GetTypeStatic() { return SceneID::TestScene1; }
	virtual SceneID GetTypeID() const noexcept override { return GetTypeStatic(); }

	virtual bool Enter() override;
	virtual bool Leave() override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	//unique_ptr<UIModule> m_uiModule;
	UIModul2* m_uiModule{ nullptr };
};