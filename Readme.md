Lake Scheme
===========

A quick and dirty scheme written in C, for fun and to use while reading The Little Schemer. Very quick and dirty, a weekend hack gone awry.

Lake Scheme uses RSR5 as a guideline but does not strictly adhere. Specifically some of the standard naming is confusing so it has been clarified. Other things have been changed just to make life in the repl more palatable.

Compiling & Running
===================

Portable C99, only dep is glib, nothing to configure, no documentation!

Install glib 2.x using your package manager, for me it's either `brew install glib` or `sudo aptitude install glib`. Once you have glib just run:

    $ make && ./lake

That will drop you at a repl. There are booleans, symbols, integers, strings, lists, dotted lists (pairs), lambdas, a few special forms, and some primitive functions.

    > #t
    #t
    > (quote symbol)
    symbol
    > '(a list of symbols)
    (a list of symbols)
    > 101
    101
    > "hello"
    "hello"
    > '(1 . 2)
    (1 . 2)
    > (lambda () "hello")
    (lambda () "hello")
    > ((lambda (x) (+ x 1)) 41)
    42

Hooray! That sure is repl-ish.

The special forms present so far are: `quote`, `define`, `set!`, `and`, `or`, and `lambda`.

    > (define answer 7)
    > answer
    7
    > (set! answer 42)
    > answer
    42
    > (define drop2 (lambda (a b . rest) rest))
    > (drop2 1 2 3 4)
    (3 4)
    > (define list (lambda rest rest))
    > (list 1 2 3 4)
    (1 2 3 4)

Woah, we even managed to define a useful function without using any primitives! There are primitives now though. So few they can be easily named. They are named thusly:

  * car, cdr, cons
  * null?
  * pair?
  * is? and equal? (is? is like eq? just renamed for clarity)
  * not
  * math operations: add, subtract, multiply, and divide (+ - * /)
  * numeric comparisons: equals, less than, greater than (= < >)

Deviating from RSR5 `eq?` is called `is?` in Lake Scheme, which I find much clearer.

    > (define a (list 1 2 3 4))
    > (define b (list 1 2 3 4))
    > (is? a b)
    #f
    > (equal? a b)
    #t

One final thing to note is an experiment I call *naked calls* (or expressions). These are only valid at the top level in the repl and are just lists without parens, like so:

    > define empty? (lambda (x) (or (null? x) (is? x 0) (equal? x "")))
    > empty? 4
    #f
    > empty? (cdr '(a))
    #t
    > empty?
    (lambda (x) (or (null? x) (is? x 0) (equal? x "")))

A naked expression with one value evaluates to the single value it contains so that typing in an atomic expression doesn't produce strange results. Due to this behaviour one caveat is that if you want to invoke a function without arguments you cannot use a naked list. When evaluating code from a file naked lists are not parsed at all.

Lake still needs:

  * primitives:
    * display values
    * compare values
    * basic control flow (if, cond, when)
    * eval and apply
    * native type operations:
      * symbol
      * boolean (logic)
      * string
      * function
      * dotted list (head, tail)
  * a minimal stdlib

There's enough here to start reading so I'm going to see where the book takes me. Other than that as much of the language as possible should be moved from C-land into Scheme-land, like the repl. The present functionality should require less C than there currently is.

Contributors
============

Just me, and I don't want any credit ;-)

License
=======

Copyright 2011 Sami Samhuri

MIT License, see included [LICENSE](blob/master/LICENSE) file