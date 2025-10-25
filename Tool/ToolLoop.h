#pragma once
#include "IRenderer.h"
#include "Shared/Framework/AppLoop.h"

struct IRenderer;
class Window;

class UserInterfaceWindow;
class TextureResBinderWindow;

namespace DX
{
	class StepTimer;
}

namespace Tool
{
	class Dialog;
	class Config;
}

class MenuBar;


class ToolLoop final : public AppLoop, public IImguiComponent
{
public:
	ToolLoop() = delete;
	ToolLoop(unique_ptr<Window> window, unique_ptr<IRenderer> renderer,
		const wstring& resourcePath, const Vector2& windowSize);
	virtual ~ToolLoop();
	virtual void Render(ImGuiIO* io) override;

	UserInterfaceWindow* GetFocusUIWindow() const noexcept;
	TextureResBinderWindow* GetFocusTextureResBinderWindow() const noexcept;
	void SetUIWindow(unique_ptr<UserInterfaceWindow> uiWindow) noexcept;
	void SetTextureWindow(unique_ptr<TextureResBinderWindow> textureWindow) noexcept;
	IRenderer* GetRenderer() const noexcept { return m_renderer; }

protected:
	virtual void Update(const DX::StepTimer& timer) override;

private:
	IRenderer* m_renderer;

	unique_ptr<Tool::Config> m_config;
	unique_ptr<MenuBar> m_menuBar;
	ImTextureID m_textureID{};

	vector<unique_ptr<UserInterfaceWindow>> m_uiWindows;
	vector<unique_ptr<TextureResBinderWindow>> m_textureWindows;
};

