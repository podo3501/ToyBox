#pragma once

enum class FileDialogType
{
	Open,  // 파일 열기
	Save   // 파일 저장
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
	class Dialog
	{
	public:
		static bool ShowFileDialog(wstring& filename, FileDialogType type);
		static void ShowInfoDialog(const DialogType dialogType, const string& m_msg) noexcept;
		static void Render() noexcept;

	private:
		static DialogType m_dialogType;
		static string m_msg;
	};
}