#pragma once

#ifndef ReturnIfFalse
#define ReturnIfFalse(x) \
	do{ \
		if(!(x)) return false; \
	} while (0)
#endif

#ifndef ReturnIfFailed
#define ReturnIfFailed(x) \
	do{ \
		if(FAILED(x)) \
			return false; \
	} while (0)
#endif

#ifndef AssertMsg
#define AssertMsg(expr, msg) \
    do { \
        if (!(expr)) { \
			__debugbreak(); \
			assert(expr && msg); \
        } \
    } while (0)
#endif

#ifndef Assert
#define Assert(expr) \
    do { \
        if (!(expr)) { \
			__debugbreak(); \
			assert(expr); \
        } \
    } while (0)
#endif

//�����Ͱ� ū ��� &�� ������ ������ �̰��� ���. const optinalRef<data>& �̷��� ���.
//�ȿ� �����ʹ� &�� ���󰡱� ������ ������ �ȵ�. ���ڷ� data�� ������ optionRef �ӽð�ü�� ����������,
//const optionalRef<data>& �� ��쿡�� optionalRef�δ� ����ȵ�. &�� ������ optionalRef�� �����.
//1. �Լ� ���ڷ� ������ = nullopt �� �ؼ� ����Ʈ ���ڷ� ������ ���������� ? : �� 3�׿����ڸ� �Ἥ ��������
//const auto& currData = data ? data->get() : defaultData;
//2. �����Ҷ� ū �����͸� �����ϸ� &�� ������ �;����µ� �̶� optionalRef�� �ؼ� ���� ������ ǥ���� �� �ִ�.
//�ƴϸ� static data empty; �� ���� �����ؾ��ϴµ� �̰� �̻��ϴϱ�.
template <typename T>
using optionalRef = std::optional<std::reference_wrapper<const T>>;
