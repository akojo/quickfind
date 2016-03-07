#include "match.h"

#include <algorithm>

using namespace std;

static int smatch(const string& str, const string& pat)
{
    auto score = 1, dist = 0;
    auto si = str.cbegin();
    auto pi = pat.cbegin();

    while (pi != pat.cend() && si != str.cend()) {
        if (tolower(*pi) == tolower(*si++)) {
            if (dist == 1) score++;
            pi++;
            dist = 0;
        }
        dist++;
    }
    return pi == pat.cend() ? score : 0;
}

vector<wstr>::iterator match(vector<wstr>::iterator begin, vector<wstr>::iterator end, const string& pattern)
{
    for_each(begin, end, [&](wstr& s) { s.score = smatch(s.str, pattern); });
    auto last = partition(begin, end, [](wstr& s) { return s.score > 0; });
    sort(begin, last);
    return last;
}
