<p align=center><img src=https://github.com/samsonjs/lake/raw/master/lake.jpg width=640 height=428 alt="Shawnigan Lake"></p>

Lake Scheme
===========

A quick and dirty scheme written in C, for fun and to use while reading The Little Schemer. Very quick and dirty, a weekend hack gone awry.

Lake Scheme uses RSR5 as a guideline but does not strictly adhere. Specifically some of the standard naming is confusing so it has been clarified. Other things have been changed just to make life in the repl more palatable.

Compiling & Running
===================

Portable C99, only dep is glib, nothing to configure, no documentation!

Install glib 2.x using your package manager, for me it's either `brew install glib` or `sudo aptitude install glib`. You'll also need some version of gcc and GNU make.

Once you have all that you can build the repl:

    $ make repl && build/repl

There are booleans, symbols, integers, strings, lists, dotted lists (pairs), lambdas, a few special forms, and some primitive functions. You can evaluate these as you expect in a Scheme repl:

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

If you want to build a static library:

    $ make lake

The binary will be in `build/lake.a` and you may do with it as you like. Lake creates no global variables and has no shared state. Everything is neatly wrapped up in a [`LakeCtx`](blob/master/src/lake.h#L120-128) so theoretically you can run multiple interpreters in the same process. I haven't tried it yet but it should work.

Tests
=====

The last interesting make task runs tests:

    $ make test

There are a few tests right now and more should be coming.

Special Forms
=============

Lake's special forms are:

  * `quote`
  * `define`
  * `set!`
  * `and`
  * `or`
  * `lambda`
  * `if`
  * `when`
  * `cond`

And an illustrative repl session:

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
    > (if #t "hooray" "sad panda")
    "hooray"
    > (when #f (nuke 'the-world)) ; never executes
    > (cond ((> 0 1) "backwards") ((< 0 1) "correct") (else "bizarre"))
    "correct"

Woah, we even managed to define a useful function without using any primitives!

Primitives
==========

There are primitives though. So few they can be easily named, and they are named thusly:

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

Naked Calls
===========

One final thing to note is an experiment called *naked calls* (or expressions). These are only valid at the top level in the repl and are just lists without parens, like so:

    > define empty? (lambda (x) (or (null? x) (is? x 0) (equal? x "")))
    > empty? 4
    #f
    > empty? (cdr '(a))
    #t
    > empty?
    (lambda (x) (or (null? x) (is? x 0) (equal? x "")))

A naked expression with one value evaluates to the single value it contains so that typing in an atomic expression doesn't produce strange results. Due to this behaviour one caveat is that if you want to invoke a function without arguments you cannot use a naked list. When evaluating code from a file naked lists are not parsed at all.

TODO
====

Lake still needs:

  * primitive functions:
    * display values
    * compare values
    * eval and apply
  * native type operations on:
    * symbol
    * boolean (logic)
    * string
    * function (eval, apply)
    * dotted list (head, tail)
  * type conversions, hopefully in a more general way than RSR5
  * a minimal stdlib

There's enough here to start reading so I'm going to see where the book takes me. Other than that as much of the language as possible should be moved from C-land into Scheme-land, like the repl. The present functionality should require less C than there currently is.

Contributors
============

Just me, and I don't want any credit ;-)

License
=======

Copyright 2011 Sami Samhuri

MIT License, see included [LICENSE](blob/master/LICENSE) file