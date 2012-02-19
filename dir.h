#ifndef DIR_H
#define DIR_H

#define DW_HIDDEN          1       /* Also hidden files and directories */
#define DW_ALL             2       /* Search both files and directories */

void dirwalk(char *name, int opts, void (*func)(char *, void *), void *ctx);

#endif /* end of include guard: DIR_H */
