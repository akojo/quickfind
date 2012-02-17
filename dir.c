#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dircat(char *d1, char *d2)
{
    char *new = malloc(strlen(d1) + strlen(d2) + 2);
    sprintf(new, "%s/%s", d1, d2);
    return new;
}

void dirwalk(char *name, void (*func)(char *, void *), void *ctx)
{
    DIR *dir = opendir(name);
    struct dirent *d;

    func(name, ctx);

    while ((d = readdir(dir)) != NULL) {
        if (d->d_type == DT_DIR && d->d_name[0] != '.') {
            char *nextdir = dircat(name, d->d_name);
            dirwalk(nextdir, func, ctx);
            free(nextdir);
        }
    }
    closedir(dir);
}


