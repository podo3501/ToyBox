#pragma once
//?!? util ������ ���� �� ���ϰ� utility.h ������ ���� �־� �ڴ�.
// map�� c++14���� string_view�� �����Ǳ� ������ ���� �̷��� �� ���� �ٷ� string_view�� �־ �ȴ�.
// ������ unordered_map�� �������̱� ������ �̷��� �����ؾ� string_view�� ����� �� �ִ�.

struct TransparentStringHash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const noexcept { return hash<std::string_view>{}(sv); }
};

template<
    typename Key,
    typename T,
    typename Hash = TransparentStringHash,
    typename KeyEqual = std::equal_to<>
>
using unordered_svmap = std::unordered_map<Key, T, Hash, KeyEqual>; 