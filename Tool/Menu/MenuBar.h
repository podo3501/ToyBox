#pragma once

class ToolSystem;
class FileTab;

namespace Tool
{
	class Popup;
}

class MenuBar
{
public:
	MenuBar() = delete;
	MenuBar(ToolSystem* toolSystem, Tool::Popup* popup);
	~MenuBar();

	void Update() const;
	bool Render() const;

private:
	unique_ptr<FileTab> m_fileTab;
};
