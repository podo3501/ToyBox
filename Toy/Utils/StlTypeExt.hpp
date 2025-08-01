#pragma once
#include <string_view>     // std::string_view
#include <unordered_map>   // std::unordered_map
#include <functional>      // std::hash, std::equal_to<>
//�� ������ type�� ������ ���̱� ������ ����� include �ص� ��� ����. 

// map�� c++14���� string_view�� �����Ǳ� ������ ���� �̷��� �� ���� �ٷ� string_view�� �־ �ȴ�.
// ������ unordered_map�� �������̱� ������ �̷��� �����ؾ� string_view�� ����� �� �ִ�.
struct TransparentStringHash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
};

template<
    typename Key,
    typename T,
    typename Hash = TransparentStringHash,
    typename KeyEqual = std::equal_to<>
>
using unordered_svmap = std::unordered_map<Key, T, Hash, KeyEqual>; 