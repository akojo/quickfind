#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <array>

#include "dir.h"

using namespace std;

namespace {

bool skip(const char *dirname, int opts) noexcept
{
    if (dirname[0] == '.') {
        return (!(opts & DW_HIDDEN)) ||
            (dirname[1] == '\0') ||
            (dirname[1] == '.' && dirname[2] == '\0');
    }
    return false;
}

int is_dir(struct dirent *d, const char *name) noexcept
{
    struct stat s;
#if defined(DT_DIR) && defined(DT_UNKNOWN)
    return d->d_type == DT_DIR ||
        (d->d_type == DT_UNKNOWN && stat(name, &s) == 0 && S_ISDIR(s.st_mode));
#else
    return stat(name, &s) == 0 && S_ISDIR(s.st_mode);
#endif
}

void walk(const string& name, int opts, FoundFunc func)
{
    auto dir = opendir(name.c_str());
    struct dirent* d;
    string path = name;
    path.push_back('/');
    auto path_end = std::distance(path.begin(), path.end());
    path.resize(FILENAME_MAX);

    if (!dir) return;

    while ((d = readdir(dir)) != NULL) {
        if (skip(d->d_name, opts)) continue;

        path.insert(path.erase(path.begin() + path_end, path.end()),
                    d->d_name,
                    d->d_name + d->d_namlen);

        if (is_dir(d, name.c_str())) {
            if (opts & DW_DIRECTORIES) func(path);
            walk(path, opts, func);
        }
        else if (opts & DW_FILES) {
            func(path);
        }
    }
    closedir(dir);
}

} // namespace

void dirwalk(string dirname, int opts, FoundFunc func)
{
    walk(dirname, opts, func);
}
