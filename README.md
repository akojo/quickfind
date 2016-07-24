# QuickFind

QuickFind is a set of two small command-line utilities utilizing the same fuzzy
matching algorithm:

* `qfind` to quickly find files and directories, and
* `qselect` to quickly select a string from a list of strings

`qfind` grew out of frustration towards the classic `find . -type f | grep`
approach when either trying match several parts of the file path, or not having a very clear
recollection of what was the exact name of the file to be searched.

`qselect` is pretty much a direct replacement for
[selecta](https://github.com/garybernhardt/selecta),
the main difference being that it should be quite a bit faster since it's been
written in C++ instead of Ruby.

## Demo
[![qfind demo](https://asciinema.org/a/2p38rdci1y6nc7f3wqud71f6l.png)](https://asciinema.org/a/2p38rdci1y6nc7f3wqud71f6l)
[![qselect demo](https://asciinema.org/a/d8zn697rk9udi7u8vcvftg5qn.png)](https://asciinema.org/a/d8zn697rk9udi7u8vcvftg5qn)

## qfind

`qfind` is very simple file/directory search program, its invocation being

```sh
qfind [-adf] [pattern] [path...]
```
Without any command line arguments `qfind` will list all files and directories
reachable from current working directory. With search pattern, it will only
return matching names. Restricting the search to certain subdirectories is done
by listing the directory names after search pattern.

Option `-a` will include also hidden files/directories, `-f` shows only files
and `-d` shows only directories.

### Examples

* `find . - type f`, i.e. all files under current directory
```sh
qfind -f
```
* Find the songs for _Black Sabbath_'s Album _Sabbath Bloody Sabbath_ from my music library
```sh
qfind -f blackbloody
```
* Find out where'd I put `qselect.cpp`
```sh
qfind -f qselectcpp
```

Incidentally, the last search takes about 300 milliseconds on my computer, inside a
source directory containing about 90000 files inside 18000 directories.

## qselect

`qselect` works pretty much like its role model `selecta`. It reads the list of search
strings from standard input, presents a search UI to the user, and then either prints
selection to its standard out or prints nothing and exits with status of `1`.

The search UI is presented by opening `/dev/tty` and reading/writing there, so that when
`qselect` is used in a pipeline, only the selected string will be written to stdout.

`qselect` has a few useful key bindinings to control the search UI:

* `Ctrl-C` will exit the program with an error status (1) and prints nothing
* `Ctrl-U` clears the search string
* `Ctrl-N` and `Ctrl-P` select the next and previous match from the presented list
* `Enter` selects the highlighted match and prints it to standard out

To search the contents of a file use the following command

```
qselect < <path to file>
```

now you can type the keywords and get the appropriate results.

## Building and installing

To build and install both programs, just run

`$ make && make install`

By default the `Makefile` will try to install the programs under `/usr`; this
can be changed using `PREFIX` environment variable, e.g.

`$ PREFIX=~/ make && make install`

## Algorithm

The programs both use the same fuzzy matching algorithm:
a weighted subsequence match, i.e. all the characters of the search string
must be present in the target string in order, but not necessarily adjacent.
As a useful heuristic the algorithm gives larger weight to input strings
that have the characters of the search string adjacent to each other.

For example, searching for "qfind" will find both `qfind` and `quickfind`,
but `qfind` will be sorted earlier because it matches exactly whereas `q` is
separate from the rest of the match characters in `quickfind`.