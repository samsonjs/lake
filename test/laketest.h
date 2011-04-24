/** 
  * laketest.h
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  * Based on MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html
  *
  */

#include <stdio.h>

void restore_output(void);

#define lt_assert(message, test) do {                              \
        if (!(test)) {                                             \
            restore_output();                                      \
            fprintf(stderr, "%s:%d assertion failed: " #test "\n", \
                    __FILE__, __LINE__);                           \
            return message;                                        \
        }                                                          \
    } while (0)

typedef char *(*test_fn)(void);

/* Returns non-zero if all passed, or zero if any failed */
int lt_run_tests(char *title, test_fn *tests);
