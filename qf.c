#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "array.h"
#include "dir.h"
#include "wstr.h"

typedef struct {
    array *dirs;
    char *pattern;
} matchdata;

int match(char *str, char *pat)
{
    int score = 1, dist = INT_MAX;

    while (*pat && *str) {
        if (tolower(*pat) == tolower(*str++)) {
            if (dist == 1) score++;
            pat++;
            dist = 0;
        }
        dist++;
    }
    return *pat == '\0' ? score : 0;
}

void matchdir(char *dirname, void *ctx)
{
    int score;
    matchdata *md = (matchdata *)ctx;

    if ((score = match(dirname, md->pattern)))
        array_push(md->dirs, wstr_new(score, dirname));
}

int namecmp(const void *n1, const void *n2)
{
    return wstr_cmp(*(wstr **)n1, *(wstr **)n2);
}

void printname(void *n)
{
    printf("%s\n", wstr_data(n));
}

void usage(void)
{
    fprintf(stderr, "Usage: qf [-af] <pattern>\n");
    fprintf(stderr, "  -a  Show also hidden files and directories\n");
    fprintf(stderr, "  -f  Search all kinds of files (default is only " \
            "directories)\n");
    fprintf(stderr, "  -h  Print this info\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    matchdata md;
    int flags = 0, opt;

    while ((opt = getopt(argc, argv, "af")) != -1) {
        switch (opt) {
        case 'a':
            flags |= DW_HIDDEN;
            break;
        case 'f':
            flags |= DW_ALL;
            break;
        case 'h':
        default:
            usage();
        }
    }

    md.dirs = array_new(1024);
    if (argc >= optind)
        md.pattern = argv[optind];
    else
        md.pattern = "";

    dirwalk(".", flags, matchdir, &md);

    array_sort(md.dirs, namecmp);
    array_foreach(md.dirs, printname);

    return 0;
}
