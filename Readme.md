Lake Scheme
===========

A quick and dirty scheme written in C, for fun and to use while reading The Little Schemer. Very quick and dirty, a weekend hack.

Compiling & Running
===================

Portable C, only dep is glib, nothing to configure, no documentation!

Install glib 2.x using your package manager, for me it's either `brew install glib` or `sudo aptitude install glib`.

    $ make
    $ ./lake

That will drop you at a repl. You can type expressions and they will be echoed back to you. There are symbols, integers, strings, and lists.

    > (foo bar 42 "hello" (nested list))
    (foo bar 42 "hello" (nested list))

Hooray! It parses (reads), evaluates, and then prints things back.

Lake needs:

  * dotted lists
  * functions
  * flesh out eval, write apply
  * booleans
  * chars
  * primitives (especially define)
    * define and friends
    * branching
    * native type operations
      * symbol
      * integer (math)
      * boolean (logic)
      * char
      * string
      * function
      * list (cons, car, cdr, ...)
      * dotted list
  * a minimal stdlib
  * sugar such as '... -> (quote ...)

I don't think I'll need any other numeric types, but they are easy to implement anyway when performance is no concern and they're all boxed.

Contributors
============

None of them want any credit.

License
=======

Copyright 2011 Sami Samhuri

MIT License, see included [LICENSE](blob/master/LICENSE) file