#pragma once

struct IRenderer;
class MainWindow;

namespace Tool
{
	class Popup;
}

namespace Tool
{
	class MainMenuBar
	{
	public:
		MainMenuBar() = delete;
		MainMenuBar(Popup* popup);
		~MainMenuBar();

		unique_ptr<MainWindow> OpenFile(IRenderer* renderer) noexcept;

	private:
		Tool::Popup* m_popup;
	};
}