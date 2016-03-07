#include <cctype>
#include <iostream>
#include "match.h"
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

int main(void)
{
    vector<wstr> input;
    string line;

    while (getline(cin, line)) input.emplace_back(line);

    FILE *output = fdopen(dup(fileno(stdout)), "w");
    freopen("/dev/tty", "r", stdin);
    freopen("/dev/tty", "w", stdout);

    initscr();
    raw();
    noecho();

    int width, height;
    getmaxyx(stdscr, width, height);

    char ch;
    string search;
    mvprintw(0, 0, "%d > ", input.size());
    while ((ch = getch()) != ERR) {
        if (ch == 3 || ch == 10) break;
        else if (ch == 127) {
            if (search.length() > 0) search.erase(search.cend() - 1);
        }
        else if (ch == 21) search.clear();
        else search += ch;

        erase();

        mvprintw(1, 0, "%s %d ", keyname(ch), ch);

        auto last = match(input.begin(), input.end(), search);
        auto it = input.cbegin();
        for (int i = 2; i < height && it != last; ++i, ++it) {
            if (i == 2) attron(A_REVERSE);
            mvprintw(i, 0, "%s", (*it).str.c_str());
            attroff(A_REVERSE);
        }

        mvprintw(0, 0, "%d > %s", (last - input.begin()), search.c_str());
    }

    endwin();

    if (ch == 10)
        fprintf(output, "%s\n", input[0].str.c_str());

    return 0;
}
