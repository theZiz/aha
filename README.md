aha - Ansi HTML Adapter
=======================

Converts ANSI escape sequences of a unix terminal to HTML code.

![aha screenshot](/screenshot.png?raw=true "aha screenshot")

Examples
========

* `aha --help | aha --black --title "the awesome aha help"> aha-help.htm`
  * Creates an HTML file of the help of aha with black background
* `colordiff oldfile.c newfile.c | aha > colordiff.htm`
  * Creates an HTML file with a colorful diff-output of two files "oldfile.c" and "newfile.c" with white background
* `ls --color=always | aha --pink > ls.htm`
 * Creates an HTML file with a colorful ls-output with pink background.
* `echo q | htop | aha --black --line-fix > htop.htm`
  * Creates an HTML file with the output of htop. You have to use --line-fix due the other new-line-commands htop uses.
* `MAN_KEEP_FORMATTING=1 COLUMNS=80 man aha | ul | aha > man-aha.htm`
  * Creates an HTML file with the man page of aha. Man uses nroff's bold and underline, which ul converts to SGR.

Compilation / Installation
==========================

Aha has no dependencies except for a C compiler and `make`.

To compile just type `make`.
To install aha to `/usr/local/` type `make install`.
You can change the installation directory with `make install PREFIX=/your/path`.
You can override the man directory with `make install MANDIR=/your/path/man`

Licensing
=========

All files are subjects to the LGPL2+ or the MPL1.1 (Dual licensed).
