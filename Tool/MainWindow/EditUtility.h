#pragma once
#include "../Toy/UserInterface/Property.h"

//edit�� ���� �ٲ�� true�� �����ϰ� �ٲ��� ������ false�� �����Ѵ�.
//&�� �ƴ� const &�� ���ڷ� ���´ٸ� ���� �ٲ�� �����Ƿ� void �Լ��� �ȴ�.

template<typename T>
concept SignedIntegral = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept UnsignedIntegral = std::is_integral_v<T> && std::is_unsigned_v<T>;

template<typename T>
bool EditInteger(const string& label, T& value)
{
    int temp = value;

    if (!ImGui::InputInt(label.c_str(), &temp)) return false;
    if constexpr (SignedIntegral<T>) {
        if (temp < std::numeric_limits<T>::min() || temp > std::numeric_limits<T>::max()) return false;
    }
    else if constexpr (UnsignedIntegral<T>) {
        if (temp < 0 || temp > std::numeric_limits<int>::max()) return false;
    }

    value = temp;
    return true;
}

struct SourceDivider;

bool EditPosition(XMINT2& position);
bool EditSize(XMUINT2& size);
bool EditRectangle(const string& label, Rectangle& rect);
bool EditRectangle(const string& label, Property<Rectangle>& rect);
bool EditCheckbox(const string& label, bool& check);
bool EditText(const string& label, string& text);
bool EditText(const string& label, wstring& text);
bool EditFilename(const string& label, wstring& filename);
bool EditFilename(const string& label, Property<wstring>& filename);
bool EditSourceAndDivider(const string& sourceLabel, const string& deviderLabel, SourceDivider& rectDivider);

bool SelectComboItem(const string& label, const vector<string>& items, string& select);