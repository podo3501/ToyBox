#pragma once

class ToolSystem;
class FileTab;

namespace Tool
{
	class Dialog;
}

class MenuBar
{
public:
	MenuBar() = delete;
	MenuBar(ToolSystem* toolSystem, Tool::Dialog* dialog);
	~MenuBar();

	void Update() const;
	bool Render() const;

private:
	unique_ptr<FileTab> m_fileTab;
};
