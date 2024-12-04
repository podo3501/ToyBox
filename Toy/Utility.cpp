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

wstring StringToWString(const string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

string RemoveNullTerminator(const string& str) 
{
	size_t end = str.find('\0');
	return (end == std::string::npos) ? str : str.substr(0, end);
}

string WStringToString(const wstring& wstr)
{
	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string str(bufferSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, nullptr, nullptr);
	return str;
}

//// µ¡¼À ¿¬»êÀÚ ¿À¹ö·Îµù
//XMINT2 operator+(const XMINT2& a, const XMINT2& b)
//{
//    return XMINT2(a.x + b.x, a.y + b.y);
//}

//// »¬¼À ¿¬»êÀÚ ¿À¹ö·Îµù
//XMINT2 operator-(const XMINT2& a, const XMINT2& b)
//{
//    return XMINT2(a.x - b.x, a.y - b.y);
//}

//// °ö¼À ¿¬»êÀÚ ¿À¹ö·Îµù
//XMINT2 operator*(const XMINT2& a, const XMINT2& b)
//{
//    return XMINT2(a.x * b.x, a.y * b.y);
//}
//
//// ³ª´°¼À ¿¬»êÀÚ ¿À¹ö·Îµù
//XMINT2 operator/(const XMINT2& a, const XMINT2& b)
//{
//    return XMINT2((b.x != 0) ? a.x / b.x : 0, (b.y != 0) ? a.y / b.y : 0);
//}
//
//XMINT2 Vector2ToXMINT2(const Vector2& rhs)
//{
//    return { static_cast<int32_t>(rhs.x), static_cast<int32_t>(rhs.y) };
//}
//
//Vector2 XMINT2ToVector2(const XMINT2& rhs)
//{
//    return { static_cast<float>(rhs.x), static_cast<float>(rhs.y) };
//}