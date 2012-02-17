#include <stdlib.h>
#include <string.h>

#include "wstr.h"

struct wstr {
    int score;
    char *str;
};

wstr *wstr_new(int score, char *str)
{
    wstr *s = malloc(sizeof(wstr));
    s->score = score;
    s->str = strdup(str);
    return s;
}

int wstr_score(wstr *str)
{
    return str->score;
}

char *wstr_data(wstr *str)
{
    return str->str;
}

int wstr_cmp(const wstr *s1, const wstr *s2)
{
    int s = s2->score - s1->score;
    if (s) return s;
    else return strcasecmp(s1->str, s2->str);
}
