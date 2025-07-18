#pragma once
#include <string_view> //hpp파일에서 이걸 인클루드 안해줘도 컴파일 되지만 vs의 intellisense가 작동이 안된다. 이 파일이 아닌 다른 파일들이 작동을 안하기 때문에 찾기 힘들다.
//?!? util 폴더를 만들어서 이 파일과 utility.h 파일을 같이 둬야 겠다.
 
// map은 c++14부터 string_view가 지원되기 때문에 굳이 이렇게 안 만들어도 바로 string_view를 넣어도 된다.
// 지금은 unordered_map은 미지원이기 때문에 이렇게 구현해야 string_view를 사용할 수 있다.
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

//set, unordered_set, map, unordered_map 에 insert를 할때 없으면 insert 있으면 false를 리턴하는 stl 헬퍼함수(이걸 쓰면 속도가 빠르다)
//다른 stl은 리턴값이 다르거나 multixxx 같은 경우는 중복키를 포함하기 때문에 리턴값이 true만 나오니 불필요하다.
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