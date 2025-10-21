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

//���� ���� ���� ǥ������ nullopt�� '��' �����ؾ� �ϴ� �κп��� �����ؾ���.
//�����Ͱ� ū ��� &�� ������ ������ �̰��� ���. const optinalRef<data>& �̷��� ���.
//�ȿ� �����ʹ� &�� ���󰡱� ������ ������ �ȵ�. ���ڷ� data�� ������ optionRef �ӽð�ü�� ����������,
//const optionalRef<data>& �� ��쿡�� optionalRef�δ� ����ȵ�. &�� ������ optionalRef�� �����.
// �� �Լ����� endPos�� �� �־����� startPos�� �ϰ� ������ endPos = startPos �̷��� ����Ʈ ���ڷ� ���� ������ ����
//�����Ҷ� ū �����͸� �����ϸ� &�� ������ �;����µ� �̶� optionalRef�� �ؼ� ���� ������ ǥ���� �� �ִ�.
//�ƴϸ� static data empty; �� ���� �����ؾ��ϴµ� �̰� �̻��ϴϱ�.
template <typename T>
using optionalRef = std::optional<std::reference_wrapper<const T>>;
