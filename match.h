#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <strings.h>
#include <vector>

struct wstr {
    int score;
    std::string str;

    template <typename T>
    wstr(T&& name) : score(0), str(std::forward<T>(name))
    {}

    bool operator<(const wstr& other) const
    {
        if (score > other.score) return true;
        if (score < other.score) return false;
        return strcasecmp(str.c_str(), other.str.c_str()) < 0;
    }
};

std::vector<wstr>::iterator match(std::vector<wstr>::iterator begin,
                             std::vector<wstr>::iterator end,
                             const std::string& pattern);

#endif // MATCH_H
