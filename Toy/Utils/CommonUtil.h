#pragma once
//이 파일은 모든 util 파일에 헤더에 포함되므로 모든 헤더에 include 된다는 생각으로 간단한 것만 추가해야 한다.

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

void TracyStartupProfiler();
void TracyShutdownProfiler();