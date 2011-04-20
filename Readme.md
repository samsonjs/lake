Lake Scheme
===========

A quick and dirty scheme written in C, for fun and to use while reading The Little Schemer. Very quick and dirty, a weekend hack gone awry.

Compiling & Running
===================

Portable C99, only dep is glib, nothing to configure, no documentation!

Install glib 2.x using your package manager, for me it's either `brew install glib` or `sudo aptitude install glib`. Once you have glib just run:

    $ make && ./lake

That will drop you at a repl. There are booleans, symbols, integers, strings, lists, dotted lists (pairs), lambdas, and a few special forms.

    > #t
    #t
    > (quote symbol)
    symbol
    > 101
    101
    > "hello"
    "hello"
    > (quote (1 2 3 4))
    (1 2 3 4)
    > (quote (1 . 2))
    (1 . 2)
    > (lambda () "hello")
    (lambda () "hello")
    > (define inc (lambda (x) (+ x 1)))

Hooray! It parses (reads), evaluates, and then prints things back. However there aren't any primitives yet, so no `+`, and if you try to *use* `inc` defined above it will not work. The special forms present so far are: `quote`, `define`, `set!`, `and`, `or`, and `lambda`.

    > (define answer 7)
    > answer
    7
    > (set! answer 42)
    > answer
    42
    > (define list (lambda rest rest))
    > (list 1 2 3 4)
    (1 2 3 4)

Woah, we even managed to define a useful function without any primitives!

Lake still needs:

  * primitives:
    * display values
    * compare values
    * basic control flow (if, cond, when)
    * native type operations:
      * symbol
      * integer (math)
      * boolean (logic)
      * string
      * function
      * list (cons, car, cdr, ...)
      * dotted list (head, tail)
  * a minimal stdlib
  * sugar such as '... -> (quote ...)

I don't think I'll need any other numeric types. Similarly I don't intend to add features expected from real languages, such as exception handling, I/O, GC, etc.

Contributors
============

None of them want any credit.

License
=======

Copyright 2011 Sami Samhuri

MIT License, see included [LICENSE](blob/master/LICENSE) file