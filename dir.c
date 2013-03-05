#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dir.h"

static int skip(char *dirname)
{
    return (dirname[0] == '.') &&
        ((dirname[1] == '\0') || (dirname[1] == '.' && dirname[2] == '\0'));
}

static void walk(char *name, int opts, void (*func)(char *, void *), void *ctx)
{
    DIR *dir = opendir(name);
	char* endp = name + strlen(name);
    struct dirent *d;

    if (!dir) return;

	*endp++ = '/';

    while ((d = readdir(dir)) != NULL) {
        if (skip(d->d_name)) continue;
        if (d->d_name[0] == '.' && !(opts & DW_HIDDEN)) continue;

        strcpy(endp, d->d_name);

		if (d->d_type == DT_DIR) {
           if (opts & DW_DIRECTORIES)
			   func(name, ctx);
           walk(name, opts, func, ctx);
		}
		else if (opts & DW_FILES) {
			func(name, ctx);
        }
    }
    closedir(dir);
}

void dirwalk(char *dirname, int opts, void (*func)(char *, void *), void *ctx)
{
	char* buf = malloc(FILENAME_MAX);
	strcpy(buf, dirname);
	walk(buf, opts, func, ctx);
}
