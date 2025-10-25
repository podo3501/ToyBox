#pragma once
#include "Shared/Framework/AppLoop.h"

struct ISceneManager;
struct IEventDispatcherManager;
class UIComponent;
class GameLoop final : public AppLoop
{
public:
	GameLoop() = delete;
	GameLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer,
		const wstring& resourcePath, const Vector2& windowSize);
	virtual ~GameLoop();

protected:
	virtual bool InitializeDerived() override;
	virtual bool DoPrepare() override;
	virtual void Update(const DX::StepTimer& timer) override;

private:
	bool AttachComponentToPanel(unique_ptr<UIComponent> component, const XMINT2& position) const noexcept;

	IRenderer* m_renderer;
	unique_ptr<ISceneManager> m_sceneManager;
	unique_ptr<IEventDispatcherManager> m_eventDispatcherManager;
};