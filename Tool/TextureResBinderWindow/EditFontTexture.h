#pragma once

enum class PendingAction;
class TexResCommandList;
class RenameNotifier;
class EditFontTexture
{
public:
	~EditFontTexture();
	EditFontTexture();

	void Update() noexcept;
	void Render();
	void SetCommandList(TexResCommandList* cmdList) noexcept { m_cmdList = cmdList; }

private:
	inline bool IsVaildFontIndex() const noexcept { return m_fontIndex >= 0 && m_fontIndex < m_fontFiles.size(); }
	wstring GetSelectFontFile() const noexcept;

	TexResCommandList* m_cmdList;
	vector<wstring> m_fontFiles;
	int m_fontIndex{ -1 };
	unique_ptr<RenameNotifier> m_renameNotifier;
};