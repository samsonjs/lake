#include <stdio.h>

#define mu_assert(message, test) do {                                        \
        if (!(test)) {                                                       \
            fprintf(stderr, "%s:%d assertion failed\n", __FILE__, __LINE__); \
            return "error: " message;                                        \
        }                                                                    \
    } while (0)

#define mu_run_test(test) do {  \
        char *message = test(); \
        tests_run++;            \
        if (message) {          \
            return message;     \
        }                       \
    } while (0)

extern int tests_run;
