#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <array>

#include "dir.h"

using namespace std;

static bool skip(const char *dirname, int opts) noexcept
{
    if (dirname[0] == '.') {
        return (!(opts & DW_HIDDEN)) ||
            (dirname[1] == '\0') ||
            (dirname[1] == '.' && dirname[2] == '\0');
    }
    return false;
}

static int is_dir(struct dirent *d, const char *name) noexcept
{
    struct stat s;
#if defined(DT_DIR) && defined(DT_UNKNOWN)
    return d->d_type == DT_DIR ||
        (d->d_type == DT_UNKNOWN && stat(name, &s) == 0 && S_ISDIR(s.st_mode));
#else
    return stat(name, &s) == 0 && S_ISDIR(s.st_mode);
#endif
}

static void walk(char *name, int opts, function<void(const char*)> func) noexcept
{
    auto dir = opendir(name);
    auto namelen = strlen(name);
    auto endp = name + namelen;
    struct dirent *d;

    if (!dir) return;

    *endp++ = '/';

    while ((d = readdir(dir)) != NULL) {
        if (skip(d->d_name, opts)) continue;
        if (strlen(d->d_name) + namelen >= FILENAME_MAX) continue;

        strcpy(endp, d->d_name);

        if (is_dir(d, name)) {
           if (opts & DW_DIRECTORIES) func(name);
           walk(name, opts, func);
        }
        else if (opts & DW_FILES) {
            func(name);
        }
    }
    closedir(dir);
}

void dirwalk(string dirname, int opts, function<void(const char*)> func)
{
    char buf[FILENAME_MAX];
    strncpy(buf, dirname.c_str(), FILENAME_MAX - 1);
    walk(buf, opts, func);
}
