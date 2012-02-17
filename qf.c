#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    int i;
    matchdata md;

    if (argc != 2) {
        fprintf(stderr, "Usage: qf <pattern>\n");
        return 1;
    }

    md.dirs = array_new(128);
    md.pattern = argv[1];

    dirwalk(".", matchdir, &md);

    array_sort(md.dirs, namecmp);
    array_foreach(md.dirs, printname);

    return 0;
}
