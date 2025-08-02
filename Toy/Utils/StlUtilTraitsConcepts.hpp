#pragma once
#include <memory>
#include <concepts>
#include <type_traits>
#include <utility>

namespace StlUtilDetail //�̿��̸� StlUtil������ ������ namespace ��.
{
	template<typename T>
	struct IsSmartPointer : std::false_type {};

	template<typename T>
	struct IsSmartPointer<unique_ptr<T>> : std::true_type {};

	template<typename T>
	struct IsSmartPointer<shared_ptr<T>> : std::true_type {};

	// traits �⺻ ���ø��� �������� ����(������ ���� ����)
	template<typename SmartPtr>
	struct SmartMake
	{
		static_assert(sizeof(SmartPtr) == 0, "SmartMake is not specialized for this type.");
	};

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

template <typename T>
concept RawPointerLike = std::is_pointer_v<T>;

template <typename SmartPtr>
concept SmartPointerLike = StlUtilDetail::IsSmartPointer<SmartPtr>::value;

template <typename T>
concept PointerLike = RawPointerLike<T> || StlUtilDetail::IsSmartPointer<T>::value;
