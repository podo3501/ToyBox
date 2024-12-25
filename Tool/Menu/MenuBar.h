#pragma once

class ToolSystem;
class FileTab;

class MenuBar
{
public:
	MenuBar() = delete;
	MenuBar(ToolSystem* toolSystem);
	~MenuBar();

	void Update() const;
	bool Render() const;

private:
	unique_ptr<FileTab> m_fileTab;
};
