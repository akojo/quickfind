#include "dir.h"
#include "match.h"

#include <algorithm>
#include <getopt.h>
#include <iostream>
#include <string.h>

using namespace std;

void usage(void)
{
    cerr << "Quickfind version 1.0" << endl;
    cerr << "Usage: qfind [-adf] <pattern> [path...]" << endl;
    cerr << "       qfind -h" << endl;
    cerr << "Where options are:" << endl;
    cerr << "  -a  Show also hidden files and directories" << endl;
    cerr << "  -d  Print matching directories" << endl;
    cerr << "  -f  Print matching files" << endl;
    cerr << "  -h  Print this info and exit" << endl;
    cerr << "Omitting both 'd' and 'f' flags is the same as specifying -df" << endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    auto flags = 0;
    int opt{};

    while ((opt = getopt(argc, argv, "adfh")) != -1) {
        switch (opt) {
        case 'a':
            flags |= DW_HIDDEN;
            break;
        case 'd':
            flags |= DW_DIRECTORIES;
            break;
        case 'f':
            flags |= DW_FILES;
            break;
        case 'h':
        case '?':
        default:
            usage();
        }
    }
    argc -= optind;
    argv += optind;

    if ((flags & (DW_DIRECTORIES | DW_FILES)) == 0) {
        flags |= DW_DIRECTORIES | DW_FILES;
    }

    vector<string> dirs = { "." };
    if (argc > 1) {
        dirs.clear();
        for (int i = 1; i < argc; ++i) {
            auto len = strlen(argv[i]);
            if (argv[i][len - 1] == '/')
                argv[i][len - 1] = '\0';
            dirs.emplace_back(argv[i]);
        }
    }

    string pattern = argc == 0 ? "" : *argv;
    vector<wstr> strings;
    for (auto& dir : dirs)
        dirwalk(dir, flags, [&](auto name) { strings.emplace_back(name); });
    auto last = match(strings.begin(), strings.end(), pattern);
    for_each(strings.begin(), last, [](auto& s) { puts(s.str.c_str()); });

    return 0;
}
