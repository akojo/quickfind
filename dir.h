#ifndef DIR_H
#define DIR_H

#include <functional>
#include <string>

#define DW_HIDDEN       1   /* Process hidden files and directories */
#define DW_DIRECTORIES  2   /* Process directories */
#define DW_FILES        4   /* Process files */

using FoundFunc = std::function<void(const std::string&)>;

void dirwalk(std::string name, int opts, FoundFunc func);

#endif /* end of include guard: DIR_H */
