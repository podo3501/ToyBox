#pragma once
#include "StlExtTraitsConcepts.hpp"

template<typename PointerType>
inline pair<unique_ptr<PointerType>, PointerType*> GetPtrs(unique_ptr<PointerType> uniquePtr)
{
	return make_pair(move(uniquePtr), uniquePtr.get());
}

//set, unordered_set, map, unordered_map �� insert�� �Ҷ� ������ insert ������ false�� �����ϴ� stl �����Լ�(�̰� ���� �ӵ��� ������)
//�ٸ� stl�� ���ϰ��� �ٸ��ų� multixxx ���� ���� �ߺ�Ű�� �����ϱ� ������ ���ϰ��� true�� ������ ���ʿ��ϴ�.
template<typename Container, typename... Args>
concept InsertReturnsPairWithBool = requires(Container & c, Args&&... args)
{
    { c.insert(std::forward<Args>(args)...) } -> std::same_as<std::pair<typename Container::iterator, bool>>;
};

template<typename Container, typename... Args>
    requires InsertReturnsPairWithBool<Container, Args...>
bool InsertIfAbsent(Container& c, Args&&... args) noexcept
{
    return c.insert(std::forward<Args>(args)...).second;
}

template<typename T>
bool Compare(const T& lhs, const T& rhs)
{
	if constexpr (!PointerLike<T>)
	{
		bool result = (lhs == rhs);
		Assert(result);
		return result;
	}

	if (!lhs && !rhs) return true;
	if (!lhs || !rhs) return false;

	bool result = (*lhs == *rhs);
	Assert(result);
	return result;
}

// ������ �����̳� ��
template <typename Container>
bool CompareSeq(const Container& lhs, const Container& rhs)
{
	using ValueType = typename Container::value_type;

	if constexpr (!PointerLike<ValueType>)
		return lhs == rhs;

	return ranges::equal(lhs, rhs, [](const ValueType& a, const ValueType& b) {
			return Compare(a, b);
		});
}

template <typename Assoc>
bool CompareAssoc(const Assoc& lhs, const Assoc& rhs)
{
	using MappedType = typename Assoc::mapped_type;

	if constexpr (!PointerLike<MappedType>)
		return lhs == rhs;

	return ranges::equal(lhs, rhs, [](const auto& a, const auto& b) {
		return a.first == b.first && Compare(a.second, b.second); });
}

template <typename MapType>
bool CompareUnorderedAssoc(const MapType& lhs, const MapType& rhs) 
{
	using MappedType = typename MapType::mapped_type;

	if constexpr (!PointerLike<MappedType>)
		return lhs == rhs;

	if (lhs.size() != rhs.size())
		return false;

	for (const auto& [key, val] : lhs)
	{
		auto it = rhs.find(key);
		if (it == rhs.end() || !Compare(val, it->second))
			return false;
	}

	return true;
}

// ���� �����̳ʸ� ������ �� traits ���
template<typename Assoc>
Assoc CopyAssoc(const Assoc& src)
{
	using MappedType = typename Assoc::mapped_type;
	static_assert(!RawPointerLike<MappedType>, "raw pointer Ÿ���� CopyAssoc���� �������� �ʽ��ϴ�.");

	if constexpr (!SmartPointerLike<MappedType>)
		return src;

	Assoc dst{};
	for (const auto& [key, value] : src)
		dst.emplace(key, value ? StlExtDetail::SmartMake<MappedType>::make(*value) : nullptr);

	return dst;
}

// try_emplace ������ first->second ��°� ��򸮰� ���� �� �ֱ� ������ helper �Լ� �ʿ�
template<typename Assoc, typename Key, typename... Args>
auto& TryEmplaceAssoc(Assoc& map, const Key& key, Args&&... args)
{
	using MappedType = typename Assoc::mapped_type;
	static_assert(!RawPointerLike<MappedType>, "raw pointer Ÿ���� TryEmplaceAssoc���� �������� �ʽ��ϴ�.");

	if constexpr (!SmartPointerLike<MappedType>)
		return map.try_emplace(key, MappedType(std::forward<Args>(args)...)).first->second;

	return map.try_emplace(key, StlExtDetail::SmartMake<MappedType>::make(std::forward<Args>(args)...)).first->second;
}

//���Ұ� ���°�� �ִ��� Ȯ��
template <typename Container, typename T>
optional<int> FindIndex(const Container& container, const T& target)
{
	auto it = ranges::find(container, target);
	if (it != container.end())
		return static_cast<int>(distance(container.begin(), it));
	return nullopt;
}
