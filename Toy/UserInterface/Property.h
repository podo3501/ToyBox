#pragma once

template <typename T>
	requires std::is_default_constructible_v<T>&& std::is_copy_assignable_v<T>
class Property
{
public:
	Property() = default;
	Property(const T& initialValue) : value(initialValue), isDirty{ true } {}
	bool operator==(const Property<T>& rhs) const noexcept { return value == rhs.value; }

	const T& Get() const { return value; }
	void Set(const T& newValue)
	{ 
		if (value != newValue)
			isDirty = true;
		value = newValue; 
	}

	operator T() const { return value; }
	Property& operator=(const T& newValue)
	{
		Set(newValue);
		return *this;
	}

	//T& operator*() { return value; }	//이 함수로 내부 값을 변경시키면 isDirty가 작동하지 않게 된다. 그래서 금지.
	const T& operator*() const { return value; }
	T* operator->() { return &value; }
	const T* operator->() const { return &value; }

	inline bool IsDirty() const noexcept { return isDirty; }
	inline void ClearDirty() noexcept { isDirty = false; }

private:
	T value;
	bool isDirty{ true };
};
