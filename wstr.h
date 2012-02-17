#ifndef WSTR_H
#define WSTR_H

struct wstr;
typedef struct wstr wstr;

wstr *wstr_new(int score, char *str);
int wstr_score(wstr *str);
char *wstr_data(wstr *str);
int wstr_cmp(const wstr *s1, const wstr *s2);

#endif /* end of include guard: WSTR_H */
