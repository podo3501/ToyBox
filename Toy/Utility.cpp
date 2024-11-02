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

std::wstring StringToWString(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}