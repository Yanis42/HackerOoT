#include "debug/debug_common/debug_common.h"

#ifndef DISABLE_DEBUG_FEATURES

s32 PrintUtils_VPrintf(PrintCallback* pfn, const char* fmt, va_list args) {
    return _Printf(*pfn, pfn, fmt, args);
}

s32 PrintUtils_Printf(PrintCallback* pfn, const char* fmt, ...) {
    s32 ret;
    va_list args;
    va_start(args, fmt);

    ret = PrintUtils_VPrintf(pfn, fmt, args);

    va_end(args);

    return ret;
}

#endif
