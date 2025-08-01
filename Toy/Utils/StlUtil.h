#pragma once
#include "CommonUtil.h"

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

//스마트 포인터 비교
template <typename T>
bool CompareUniquePtr(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	if (lhs == nullptr && rhs == nullptr)
		return true;

	if (lhs == nullptr || rhs == nullptr)
		return false;

	auto result{ *lhs == *rhs }; // 가리키는 값 비교
	Assert(result);

	return result;
}

//시퀀스 컨테이너 비교
template <typename T>
bool CompareSeq(const vector<unique_ptr<T>>& lhs, const vector<unique_ptr<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	return equal(lhs.begin(), lhs.end(), rhs.begin(), CompareUniquePtr<T>);
}

//원소가 몇번째에 있는지 확인
template <typename Container, typename T>
optional<int> FindIndex(const Container& container, const T& target)
{
	auto it = ranges::find(container, target);
	if (it != container.end())
		return static_cast<int>(distance(container.begin(), it));
	return nullopt;
}

//연관 컨테이너 비교
template <typename Key, typename Value>
bool CompareAssoc(const map<Key, unique_ptr<Value>>& lhs, const map<Key, unique_ptr<Value>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	auto it1 = lhs.begin();
	auto it2 = rhs.begin();

	while (it1 != lhs.end() && it2 != rhs.end()) {
		// 키 비교
		if (it1->first != it2->first)
			return false;

		// 값 비교
		if (!CompareUniquePtr(it1->second, it2->second))
			return false;

		++it1;
		++it2;
	}

	return true;
}

template <typename MapType>
bool CompareUnorderedAssoc(const MapType& lhs, const MapType& rhs) 
{
	if (lhs.size() != rhs.size())
		return false;

	for (const auto& [key, val] : lhs) {
		auto it = rhs.find(key);
		if (it == rhs.end())
			return false;

		if (!CompareUniquePtr(val, it->second))
			return false;
	}

	return true;
}

namespace StlUtilDetail //이왕이면 이 파일 안에서 쓸려고 namespace를 사용했다.
{
	// traits 기본 템플릿은 정의하지 않음(컴파일 에러 유도)
	template<typename SmartPtr>
	struct SmartMake;

	// unique_ptr 특수화
	template<typename T>
	struct SmartMake<unique_ptr<T>>
	{
		template<typename... Args>
		static unique_ptr<T> make(Args&&... args)
		{
			return make_unique<T>(forward<Args>(args)...);
		}
	};

	// shared_ptr 특수화
	template<typename T>
	struct SmartMake<shared_ptr<T>>
	{
		template<typename... Args>
		static shared_ptr<T> make(Args&&... args)
		{
			return make_shared<T>(forward<Args>(args)...);
		}
	};
}

// 연관 컨테이너를 복사할 때 traits 사용
template<typename Assoc>
Assoc CopyAssoc(const Assoc& src)
{
	using SmartPtr = typename Assoc::mapped_type;

	Assoc dst{};
	for (const auto& [key, value] : src)
		dst.emplace(key, value ? StlUtilDetail::SmartMake<SmartPtr>::make(*value) : nullptr);

	return dst;
}

// unique, shared 작동되는 try_emplace 구문상 first->second 라는게 헤깔리게 만들 수 있기 때문에 helper 함수 필요
template<typename Assoc, typename Key, typename... Args>
auto& TryEmplaceAssoc(Assoc& map, const Key& key, Args&&... args)
{
	using SmartPtr = typename Assoc::mapped_type;
	return map.try_emplace(key, StlUtilDetail::SmartMake<SmartPtr>::make(std::forward<Args>(args)...)).first->second;
}
