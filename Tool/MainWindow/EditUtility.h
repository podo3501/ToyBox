#pragma once
#include "../Toy/UserInterface/Property.h"

template<typename T>
concept SignedIntegral = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept UnsignedIntegral = std::is_integral_v<T> && std::is_unsigned_v<T>;

template<typename T>
bool EditInteger(const char* label, T& value)
{
    int temp = value;

    if (!ImGui::InputInt(label, &temp)) return false;
    if constexpr (SignedIntegral<T>) {
        if (temp < std::numeric_limits<T>::min() || temp > std::numeric_limits<T>::max()) return false;
    }
    else if constexpr (UnsignedIntegral<T>) {
        if (temp < 0 || temp > std::numeric_limits<int>::max()) return false;
    }

    value = temp;
    return true;
}

bool EditRectangle(const char* label, Property<Rectangle>& rect);
void EditFilename(const string& label, Property<wstring>& filename);