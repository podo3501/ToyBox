#pragma once
#include <atomic>

//���� �����Ͱ� �ö󰡸� ���۷���ī���͸� ���ؼ� �����Ǵ� �κ��� ���ٸ� ����� �ְ� �����ִ� ���� Ŭ����
template<typename T>
struct RefCounted
{
	T data;

	void IncRef() const noexcept { refCount.fetch_add(1, std::memory_order_relaxed); }
	bool DecRef() const noexcept { return refCount.fetch_sub(1, std::memory_order_acq_rel) == 1; }
	unsigned int GetRefCount() const noexcept { return refCount.load(std::memory_order_relaxed); }

private:
	mutable std::atomic<unsigned int> refCount{ 0 };
};