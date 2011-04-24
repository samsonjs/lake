/** 
  * laketest.c
  * Lake Scheme
  *
  * Copyright 2011 Sami Samhuri
  * MIT License
  *
  * Based on MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html
  * 
  */

#include <stdio.h>
#include "laketest.h"

static void capture_output(void)
{
    int fd = fopen("./tmp", "w");
    close(2);
    int newfd = dup(fd);
    close(fd);
    
    fd = fopen("./tmp", "w");
    close(1);
    newfd = dup(fd);
    close(fd);
}

void restore_output(void)
{
    freopen("/dev/tty", "a", stdout);
    freopen("/dev/tty", "a", stderr);
    unlink("./tmp");
}

int lt_run_tests(char *title, test_fn *tests)
{
    int pass = 0;
    int n_tests = 0;
    char *message = 0;
    test_fn test;
    printf("-- %s --\n", title);
    capture_output();
    while ((test = *(tests++))) {
        if ((message = test())) break;
        n_tests++;
    }
    restore_output();
    pass = message == 0;
    if (pass) {
        fprintf(stderr, "PASS: %d test%s\n", n_tests, n_tests == 1 ? "" : "s");
    }
    else {
        fprintf(stderr, "FAIL: %s\n", message);
    }
    return pass;
}
