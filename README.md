QuickFind
=========

Quickfind (or `qf`) is a UN\*X tool, inspired by TextMate's Command-T plugin, for
quickly finding files and directories under your working directory. I primarily
use it as a part of a fast approximate `cd` command and as a powerful
alternative for the `find .  | grep pattern` idiom.

Using QuickFind
---------------

To find something, just type:

```shell
$ qf [-fa] pattern
```

By default `qf` ignores all hidden directories/files and only prints out paths
of directories. You can make `qf` show hidden files and directories with the
`-a` switch. To show also files in addition of directories with the `-f` switch.

`qf` Uses a very simple pattern matching algorithm to find what you are looking
for. You specify a pattern and then `qf` finds all directories whose pathname
includes all the letters (case insensitive) in the pattern, in the given order.
The letters need not be consecutive in the path, but more weight is given to
paths where the letters of the pattern are next to each other.

For example, if I were to find the location of all ABBA albums in my iTunes
library:

```shell
iTunes$ qf abba
./Music/ABBA
./Music/ABBA/Gold_ Greatest Hits
./Books/Edwin Abbott Abbott
./Music/Black Sabbath
./Music/Black Sabbath/Sabbath Bloody Sabbath
./Music/Nat Newborn/Back To The Moon
./Music/Carbonne - Di Piazza - Manring/Carbonne - Di Piazza - Manring
./Music/Iron Maiden/The Number of the Beast
./Music/Laura Branigan/The Best of Branigan
./Music/Manu Dibango/African Soul - The Very Best of Manu Dibango
./Music/The Cranberries/Everybody Else Is Doing It, So Why Can't We_
```

As you can see, all the results contain the letters "ABBA" in order, but the
paths where the letters are consecutive were given first.

Or, maybe I'd like to see the tracks on the Black Sabbath's 'Sabbath Bloody
Sabbath' album:

```shell
iTunes$ qf -f sabbblosabb.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/01 Sabbath Bloody Sabbath.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/02 A National Acrobat.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/03 Fluff.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/04 Sabbra Cadabra.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/05 Killing Yourself to Live.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/06 Who Are You_.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/07 Looking for Today.m4a
./Music/Black Sabbath/Sabbath Bloody Sabbath/08 Spiral Architect.m4a
```

Note that in the last example I had to quote the search result since the
pathname contained spaces.

To use `qf` as a part of an approximate `cd` command, I've included a sample
shell function `acd` in the file `acd.sh`. Just copy the function somewhere
where your shell will load it during startup (like `.profile`, `.bashrc` etc.)
and you have a quick way to go anywhere under your current working directory
without having to type full pathnames.

Building and Installing QuickFind
---------------------------------

```shell
make
make install
```

By default `qf` installs into `/usr/bin`, but you can easily change the location
by giving a different prefix during installation:

```shell
PREFIX=~/bin make install
```

Similarly, since the `qf` Makefile only uses implicit rules, you can control the
C compiler and the compiler flags with environment variables. Maybe you'd like
to compile `qf` with `clang` with full optimizations:

```shell
CC=clang CFLAGS=-O4 make
```

You get the idea.

Removing `qf` is as simple as a `make uninstall`. Just remember to give the same
prefix that you used during installation since the Makefile doesn't remember it
(maybe it should).
