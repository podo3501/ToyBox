#pragma once

//���� �����Ͱ� �ö󰡸� ���۷���ī���͸� ���ؼ� �����Ǵ� �κ��� ���ٸ� ����� �ְ� �����ִ� ���� Ŭ����
//�����忡 �����ϰ� �ҷ��� atmic�� �������.
//delete this�� �Ἥ �ڵ����� �ڽ��� �������� �� ���� �ִµ� �� �����͸� ����ϴ� �κ��� ���� map�̶��
//�ʾȿ� �����Ͱ� �˾Ƽ� �������� ��� �����Ͱ� �ȴ�. ���� ���������� ��� �ڵ��� ���� ������, �׷����̶��
//�׳� DecRef���� ��ȯ������ ���� �����͸� ������ ������ �����ϴ� ���� ���ٰ� �����Ǿ���. ���� map����
//���� Ŭ������ ������̶�� stl ���� �Լ��� �� �����ؾ� �Ѵ�.
template<typename T>
struct RefCounted
{
	void IncRef() const noexcept { ++refCount; }
	bool DecRef() const noexcept 
	{ 
		if (refCount == 0) return false;
		--refCount;
		return true;
	}
	unsigned int GetRefCount() const noexcept { return refCount; }

	T* operator->() { return &data; }
	const T* operator->() const { return &data; }

	T& operator*() noexcept { return data; }
	const T& operator*() const noexcept { return data; }

	T& Get() noexcept { return data; }
	const T& Get() const noexcept { return data; }

private:
	T data{};
	mutable unsigned int refCount{ 0 };
};