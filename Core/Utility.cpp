#include "pch.h"
#include "Utility.h"

CException::CException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber) :
    ErrorCode(hr),
    FunctionName(functionName),
    Filename(filename),
    LineNumber(lineNumber)
{}

std::wstring CException::ToString() const
{
    // Get the string description of the error code.
    _com_error err(ErrorCode);
    std::wstring msg = err.ErrorMessage();

    return FunctionName + L" failed in " + Filename + L"; line " + std::to_wstring(LineNumber) + L"; error: " + msg;
}

////////////////////////////////////////////////////////////////
//CycleIterator

CycleIterator::CycleIterator(int s, int e) : start(s), end(e), current(s)
{
    assert(s < e);
}

int mod(int value, int range)
{
    return (value % range + range) % range;
}

int CycleIterator::GetCurrent() const noexcept
{
    return current;
}

int CycleIterator::Increase() noexcept
{
    current = start + mod(current + 1 - start, end - start);
    return current;
}

int CycleIterator::Decrease() noexcept
{
    current = start + mod(current - 1 - start, end - start);
    return current;
}
