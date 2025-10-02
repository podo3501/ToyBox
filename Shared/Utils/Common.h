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
