#pragma once
#include "CommonUtil.h"

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

//����Ʈ ������ ��
template <typename T>
bool CompareUniquePtr(const unique_ptr<T>& lhs, const unique_ptr<T>& rhs)
{
	if (lhs == nullptr && rhs == nullptr)
		return true;

	if (lhs == nullptr || rhs == nullptr)
		return false;

	auto result{ *lhs == *rhs }; // ����Ű�� �� ��
	Assert(result);

	return result;
}

//������ �����̳� ��
template <typename T>
bool CompareSeq(const vector<unique_ptr<T>>& lhs, const vector<unique_ptr<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	return equal(lhs.begin(), lhs.end(), rhs.begin(), CompareUniquePtr<T>);
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

//���� �����̳� ��
template <typename Key, typename Value>
bool CompareAssoc(const map<Key, unique_ptr<Value>>& lhs, const map<Key, unique_ptr<Value>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;

	auto it1 = lhs.begin();
	auto it2 = rhs.begin();

	while (it1 != lhs.end() && it2 != rhs.end()) {
		// Ű ��
		if (it1->first != it2->first)
			return false;

		// �� ��
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

namespace StlUtilDetail //�̿��̸� �� ���� �ȿ��� ������ namespace�� ����ߴ�.
{
	// traits �⺻ ���ø��� �������� ����(������ ���� ����)
	template<typename SmartPtr>
	struct SmartMake;

	// unique_ptr Ư��ȭ
	template<typename T>
	struct SmartMake<unique_ptr<T>>
	{
		template<typename... Args>
		static unique_ptr<T> make(Args&&... args)
		{
			return make_unique<T>(forward<Args>(args)...);
		}
	};

	// shared_ptr Ư��ȭ
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

// ���� �����̳ʸ� ������ �� traits ���
template<typename Assoc>
Assoc CopyAssoc(const Assoc& src)
{
	using SmartPtr = typename Assoc::mapped_type;

	Assoc dst{};
	for (const auto& [key, value] : src)
		dst.emplace(key, value ? StlUtilDetail::SmartMake<SmartPtr>::make(*value) : nullptr);

	return dst;
}

// unique, shared �۵��Ǵ� try_emplace ������ first->second ��°� ��򸮰� ���� �� �ֱ� ������ helper �Լ� �ʿ�
template<typename Assoc, typename Key, typename... Args>
auto& TryEmplaceAssoc(Assoc& map, const Key& key, Args&&... args)
{
	using SmartPtr = typename Assoc::mapped_type;
	return map.try_emplace(key, StlUtilDetail::SmartMake<SmartPtr>::make(std::forward<Args>(args)...)).first->second;
}
