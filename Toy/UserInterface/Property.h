#pragma once

template <typename T>
	requires std::is_default_constructible_v<T>&& std::is_copy_assignable_v<T>
class Property
{
public:
	Property() = default;
	Property(const T& initialValue) : value(initialValue) {}
	bool operator==(const Property<T>& rhs) const noexcept { return value == rhs.value; }

	const T& Get() const { return value; }
	void Set(const T& newValue) { value = newValue; }

	operator T() const { return value; }
	Property& operator=(const T& newValue)
	{
		Set(newValue);
		return *this;
	}

	T& operator*() { return value; }
	const T& operator*() const { return value; }
	T* operator->() { return &value; }
	const T* operator->() const { return &value; }

private:
	T value;
};
