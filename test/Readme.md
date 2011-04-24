Running Tests
=============

From the root or test directory, run all tests:

    $ make test

From the test directory run individual tests:

    $ make test_eval && ./test_eval

Test Suite
==========

A custom, minimal test framework based on [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html). The entire framework is ~30 lines. Its use is very simple:

    // Shared state comes in handy
    static this_t *this;
    
    // Tests are just like MinUnit except we use lt_assert
    static char *test_this(void)
    {
        lt_assert("this should not be NULL", this != NULL);
        ...
        return 0;
    }
    
    static char *test_that(void)
    {
        lt_assert("that should be greater than zero", this->that > 0);
        return 0;
    }
    
    int main(int argc, char const *argv[])
    {
        // do your setup
        this = create_this();
        
        // run the tests exiting with 0 if all passed, non-zero if any failed
        return !lt_run_tests("Description", (test_fn[]){
            test_this,
            ...
            test_that,
            NULL         // this terminator is important
        });
    }
