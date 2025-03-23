#pragma once
//헤더가 없는 이유는 각 프로젝트에서 pch 등등에 헤더가 들어있다고 가정하기 때문이다. 오류가 난다면 여기말고 각자의 프로젝트에서 include 하자.

template <typename T>
using optionalRef = std::optional<std::reference_wrapper<const T>>;