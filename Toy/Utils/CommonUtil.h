#pragma once
//�� ������ ��� util ���Ͽ� ����� ���ԵǹǷ� ��� ����� include �ȴٴ� �������� ������ �͸� �߰��ؾ� �Ѵ�.

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