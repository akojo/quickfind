#ifndef ARRAY_H
#define ARRAY_H

struct array;
typedef struct array array;

array *array_new(int size);
void array_push(array *a, void *entry);
void array_foreach(array *a, void (*func)(void *));
void array_sort(array *a, int (*cmp)(const void *, const void *));

#endif /* end of include guard: ARRAY_H */
