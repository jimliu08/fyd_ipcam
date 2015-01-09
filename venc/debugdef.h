
#ifndef _DEBUGDEF_H_
#define _DEBUGDEF_H_

#include "dbgdefs.h"

int debug_level = 2;

#ifdef _DEBUG
#define DBPRINT_FUNC \
{\
    if(debug_level > 1)\
    printf("%s, %d, %s, ", __FILE__, __LINE__, __func__);\
}

#define VDBPRINT0(S) {DBPRINT_FUNC; DBPRINT0(S);}
#define VDBPRINT1(S, S1) {DBPRINT_FUNC; DBPRINT0(S, S1);}
#define VDBPRINT2(S, S1, S2) {DBPRINT_FUNC; DBPRINT0(S, S1, S2);}
#define VDBPRINT3(S, S1, S2, S3) {DBPRINT_FUNC; DBPRINT0(S, S1, S2, S3);}
#define VDBPRINT4(S, S1, S2, S3, S4) {DBPRINT_FUNC; DBPRINT0(S, S1, S2, S3, S4);}
#else
#define VDBPRINT0(S)
#define VDBPRINT1(S, S1)
#define VDBPRINT2(S, S1, S2)
#define VDBPRINT3(S, S1, S2, S3)
#define VDBPRINT4(S, S1, S2, S3, S4)
#endif

#endif // _DEBUGDEF_H_
