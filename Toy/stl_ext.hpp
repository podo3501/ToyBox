#pragma once
#include <string_view> //hpp���Ͽ��� �̰� ��Ŭ��� �����൵ ������ ������ vs�� intellisense�� �۵��� �ȵȴ�. �� ������ �ƴ� �ٸ� ���ϵ��� �۵��� ���ϱ� ������ ã�� �����.
//?!? util ������ ���� �� ���ϰ� utility.h ������ ���� �־� �ڴ�.
 
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

//set, unordered_set, map, unordered_map �� insert�� �Ҷ� ������ insert ������ false�� �����ϴ� stl �����Լ�(�̰� ���� �ӵ��� ������)
//�ٸ� stl�� ���ϰ��� �ٸ��ų� multixxx ���� ���� �ߺ�Ű�� �����ϱ� ������ ���ϰ��� true�� ������ ���ʿ��ϴ�.
template<typename Container, typename... Args>
concept InsertReturnsPairWithBool = requires(Container & c, Args&&... args)
{
    { c.insert(std::forward<Args>(args)...) } -> std::same_as<std::pair<typename Container::iterator, bool>>;
};

template<typename Container, typename... Args>
    requires InsertReturnsPairWithBool<Container, Args...>
bool InsertIfAbsent(Container& c, Args&&... args)
{
    return c.insert(std::forward<Args>(args)...).second;
}