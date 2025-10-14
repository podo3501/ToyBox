#pragma once
#include <atomic>

//같은 데이터가 올라가면 레퍼런스카운터를 통해서 공유되는 부분이 없다면 지울수 있게 도와주는 헬퍼 클래스
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