#pragma once

enum class PendingAction;
class TextureSourceBinder;
class RenameNotifier;
class EditFontTexture
{
public:
	~EditFontTexture();
	EditFontTexture();

	void Update() noexcept;
	void Render();
	bool SetSourceBinder(TextureSourceBinder* sourceBinder) noexcept;

private:
	inline bool IsVaildFontIndex() const noexcept { return m_fontIndex >= 0 && m_fontIndex < m_fontFiles.size(); }
	wstring GetSelectFontFile() const noexcept;

	TextureSourceBinder* m_sourceBinder;
	vector<wstring> m_fontFiles;
	int m_fontIndex{ -1 };
	unique_ptr<RenameNotifier> m_renameNotifier;
};