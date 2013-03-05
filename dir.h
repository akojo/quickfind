#ifndef DIR_H
#define DIR_H

#define DW_HIDDEN       1   /* Process hidden files and directories */
#define DW_DIRECTORIES  2   /* Process directories */
#define DW_FILES        4   /* Process files */

void dirwalk(char *name, int opts, void (*func)(char *, void *), void *ctx);

#endif /* end of include guard: DIR_H */
