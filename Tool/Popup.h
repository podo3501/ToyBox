#pragma once

enum class FileDialogType
{
	Open,  // ���� ����
	Save   // ���� ����
};

enum class DialogType
{
	Init,
	Alert,
	Message,
	Error
};

namespace Tool
{
	class Popup
	{
	public:
		Popup();
		~Popup();

		bool ShowFileDialog(wstring& filename, FileDialogType type);
		void ShowInfoDialog(const DialogType dialogType, const string& m_msg) noexcept;
		void Render() noexcept;

	private:
		DialogType m_dialogType{ DialogType::Init };
		string m_msg;
	};
}