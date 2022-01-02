#ifndef TESTUTILS_H_
#define TESTUTILS_H_
#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cmath>
extern "C" {
#else
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#endif

#define LOGERROR(_out_, _place_, _line_, ...) if (_out_) { \
    fprintf(_out_, __VA_ARGS__); fprintf(_out_, " @ %s, %d\n", _place_, _line_); }

#define LOGERRORBREAK(_out_, _place_, _line_, ...) if (_out_) { \
    fprintf(_out_, __VA_ARGS__); fprintf(_out_, " @ %s, %d\n", _place_, _line_); break; }

#define TESTSIGNF(var) (((var) > 0.0f) ? (1.0f) : (-1.0f))

#define TESTEQUALF0(var0, var1, tol) ( \
    (fabsf((var0) - (var1)) / fabsf((var0) + (var1))) < tol)

#define TESTEQUALF(var0, var1, tol) ( \
    (fabsf(var0 + var1) < (tol)) ? TESTEQUALF0(var0 + TESTSIGNF(var0 + var1) * tol, var1 + TESTSIGNF(var0 + var1) * tol, tol) : TESTEQUALF0(var0, var1, tol))

#define TESTEQUALP3V(v0, v1, tol) ( \
    TESTEQUALF((v0)[0]/(v0)[3], (v1)[0]/(v1)[3], tol) && TESTEQUALF((v0)[1]/(v0)[3], (v1)[1]/(v1)[3], tol) && \
    TESTEQUALF((v0)[2]/(v0)[3], (v1)[2]/(v1)[3], tol))

#define TESTEQUALP2V(v0, v1, tol) ( \
    TESTEQUALF((v0)[0]/(v0)[2], (v1)[0]/(v1)[2], tol) && TESTEQUALF((v0)[1]/(v0)[2], (v1)[1]/(v1)[2], tol))

#define TESTEQUALR3V(v0, v1, tol) ( \
    TESTEQUALF((v0)[0], (v1)[0], tol) && TESTEQUALF((v0)[1], (v1)[1], tol) && TESTEQUALF((v0)[2], (v1)[2], tol))

#define TESTEQUALR2V(v0, v1, tol) ( \
    TESTEQUALF((v0)[0], (v1)[0], tol) && TESTEQUALF((v0)[1], (v1)[1], tol))
#ifdef __cplusplus
}
#endif
#endif /*  TESTUTILS_H_ */