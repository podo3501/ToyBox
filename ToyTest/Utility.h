#pragma once

#if defined(DEBUG) | defined(_DEBUG)
void ReportLiveObjects();
#endif

#ifndef ReturnIfFalse
#define ReturnIfFalse(x)                       \
{                                                             \
    if(!x) return false;                          \
}
#endif

#ifndef ReturnIfFailed
#define ReturnIfFailed(x)                                              \
{                                                                     \
    if(FAILED(x))                                            \
        return false;                                                \
}
#endif
