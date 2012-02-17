#ifndef DIR_H
#define DIR_H

void dirwalk(char *name, void (*func)(char *, void *), void *ctx);

#endif /* end of include guard: DIR_H */
