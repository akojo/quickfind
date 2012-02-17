#include <stdlib.h>

#include "array.h"

struct array {
    int idx;
    int size;
    void **entries;
};

array *array_new(int size)
{
    array *a = malloc(sizeof(array));
    a->idx = 0;
    a->size = size;
    a->entries = malloc(size * sizeof(void *));
    return a;
}

void array_push(array *a, void *entry)
{
    if (a->idx == a->size) {
        int newsize = a->size * 2;
        a->entries = realloc(a->entries, newsize * sizeof(void *));
        a->size = newsize;
    }
    a->entries[a->idx++] = entry;
}

void array_foreach(array *a, void (*func)(void *))
{
    int i;
    for (i = 0; i < a->idx; i++)
        func(a->entries[i]);
}

void array_sort(array *a, int (*cmp)(const void *, const void *))
{
    qsort(a->entries, a->idx, sizeof(void *), cmp);
}

