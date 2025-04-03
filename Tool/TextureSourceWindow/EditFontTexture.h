#pragma once

enum class PendingAction;
class TextureResourceBinder;
class TexResCommandList;
class RenameNotifier;
class EditFontTexture
{
public:
	~EditFontTexture();
	EditFontTexture();

	void Update() noexcept;
	void Render();
	bool SetBinderAndCmdList(TextureResourceBinder* resBinder, TexResCommandList* cmdList) noexcept;

private:
	inline bool IsVaildFontIndex() const noexcept { return m_fontIndex >= 0 && m_fontIndex < m_fontFiles.size(); }
	wstring GetSelectFontFile() const noexcept;

	TextureResourceBinder* m_resBinder;
	TexResCommandList* m_cmdList;
	vector<wstring> m_fontFiles;
	int m_fontIndex{ -1 };
	unique_ptr<RenameNotifier> m_renameNotifier;
};