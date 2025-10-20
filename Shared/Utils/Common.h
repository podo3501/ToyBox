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

//데이터가 큰 경우 &로 보내고 싶을때 이것을 사용. const optinalRef<data>& 이렇게 사용.
//안에 데이터는 &로 날라가기 때문에 값복사 안됨. 인자로 data를 날리면 optionRef 임시객체는 생성되지만,
//const optionalRef<data>& 인 경우에는 optionalRef로는 복사안됨. &가 없으면 optionalRef가 복사됨.
//1. 함수 인자로 보낼때 = nullopt 로 해서 디폴트 인자로 보내면 내부적으로 ? : 인 3항연산자를 써서 구현가능
//const auto& currData = data ? data->get() : defaultData;
//2. 리턴할때 큰 데이터를 리턴하면 &로 보내고 싶어지는데 이때 optionalRef로 해서 값이 없음을 표현할 수 있다.
//아니면 static data empty; 를 만들어서 리턴해야하는데 이건 이상하니까.
template <typename T>
using optionalRef = std::optional<std::reference_wrapper<const T>>;
