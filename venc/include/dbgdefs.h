/* =========================================================================================== */
#ifndef __DBGDEFS_H__
#define __DBGDEFS_H__

/* =========================================================================================== */
#ifdef _DEBUG
#include <stdio.h>
#include <stdarg.h>
static inline void debug_printf(const char *fmt,...)
{
  va_list args;
  va_start (args, fmt);
  vfprintf(stderr, fmt, args);
  va_end (args);
}
#define DBPRINT0(S)	printf(S);
#define DBPRINT1(S, S1)	printf(S, S1);
#define DBPRINT2(S, S1, S2)	printf(S, S1, S2);
#define DBPRINT3(S, S1, S2, S3)	printf(S, S1, S2, S3);
#define DBPRINT4(S, S1, S2, S3, S4)	printf(S, S1, S2, S3, S4);
#else
#define debug_printf(...)
#define DBPRINT0(S)
#define DBPRINT1(S, S1)
#define DBPRINT2(S, S1, S2)
#define DBPRINT3(S, S1, S2, S3)
#define DBPRINT4(S, S1, S2, S3, S4)
#endif

/* =========================================================================================== */
#endif //__DBGDEFS_H__
