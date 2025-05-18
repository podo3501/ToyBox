#pragma once

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
