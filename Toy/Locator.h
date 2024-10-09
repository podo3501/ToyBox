#pragma once

template<typename T>
class Locator
{
public:
	static const T* GetService() noexcept { return m_service; }
	static void Provide(T* service) { m_service = service; }

private:
	//������ �Ҹ��ڰ� ȣ����� �ʱ� ������ �������� ��������� ������ �־�� �Ѵ�.
	static T* m_service;
};

template<typename T>
T* Locator<T>::m_service{ nullptr };