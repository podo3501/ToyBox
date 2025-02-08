#include "pch.h"
#include "Utility.h"

//CException::CException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
//    ErrorCode(hr),
//    FunctionName(functionName),
//    Filename(filename),
//    LineNumber(lineNumber)
//{}
//
//std::wstring CException::ToString() const
//{
//    // Get the string description of the error code.
//    _com_error err(ErrorCode);
//    std::wstring msg = err.ErrorMessage();
//
//    return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
//}

wstring StringToWString(const string& str) noexcept
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

string RemoveNullTerminator(const string& str) noexcept
{
	size_t end = str.find('\0');
	return (end == std::string::npos) ? str : str.substr(0, end);
}

string WStringToString(const wstring& wstr) noexcept
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
	return str;
}

void StringToChar(const string& str, span<char> outstr) noexcept
{
	strncpy_s(outstr.data(), outstr.size() - 1, str.c_str(), _TRUNCATE);
	outstr[outstr.size() - 1] = '\0';
}

void WStringToChar(const wstring& wstr, span<char> outstr) noexcept
{
	StringToChar(WStringToString(wstr), outstr);
}

wstring CharToWString(const span<char> str) noexcept
{
	return StringToWString(string(str.data(), str.size()));
}

wstring IntToWString(int value) noexcept
{
	return StringToWString(to_string(value));
}