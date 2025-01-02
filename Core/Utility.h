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

template <typename E>
constexpr auto
Ev(E enumerator) noexcept
{
    return static_cast<std::underlying_type_t<E>>(enumerator);
}

class CycleIterator 
{
public:
    CycleIterator(int s, int e);

    int Increase() noexcept;
    int Decrease() noexcept;
    int GetCurrent() const noexcept;

private:
    int current{}, start{}, end{};
};

void MergeRectangles(vector<Rectangle>& rects) noexcept;