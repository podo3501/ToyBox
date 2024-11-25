#pragma once

namespace Tool
{
	class Popup
	{
	public:
		Popup();
		~Popup();

		bool OpenFileDialog(wstring& filename);
		void ShowErrorDialog(const string& errorMsg) noexcept;
		void Render() noexcept;

	private:
		bool m_errorDialog{ false };
		string m_errorMsg;
	};
}