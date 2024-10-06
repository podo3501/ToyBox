#pragma once

class CException
{
public:
    CException() = default;
    CException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

    std::wstring ToString()const;

    HRESULT ErrorCode = S_OK;
    std::wstring FunctionName;
    std::wstring Filename;
    int LineNumber = -1;
};

#ifndef ReturnIfFalse
#define ReturnIfFalse(x)                       \
{                                                             \
    if(!x) return false;                          \
}
#endif

#ifndef ReturnIfFailed
#define ReturnIfFailed(x)                                              \
{                                                                     \
    if(FAILED(x))                                            \
        return false;                                                \
}
#endif

template<typename T>
class Locator
{
public:
	static T* GetService() { return m_service; }
	static void Provide(T* service) { m_service = service; }

private:
    //생성자 소멸자가 호출되지 않기 때문에 변수값을 명시적으로 관리해 주어야 한다.
	static T* m_service;
};

template<typename T>
T* Locator<T>::m_service{ nullptr };