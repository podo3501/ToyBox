#pragma once
//?!? util 폴더를 만들어서 이 파일과 utility.h 파일을 같이 둬야 겠다.
// map은 c++14부터 string_view가 지원되기 때문에 굳이 이렇게 안 만들어도 바로 string_view를 넣어도 된다.
// 지금은 unordered_map은 미지원이기 때문에 이렇게 구현해야 string_view를 사용할 수 있다.

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