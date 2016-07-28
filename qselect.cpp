#include "match.h"
#include "term.h"
#include "prompt.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const int MAXLINES = 20;

int main(void)
{
    vector<wstr> input;
    string line;

    while (getline(cin, line)) input.emplace_back(wstr{line});

    Term tty("/dev/tty");
    Term::Size screen_size = tty.get_screen_size();

    char ch;
    int selection = 1;
    Prompt prompt(tty, "> ");

    tty.erase_display(Term::FORWARD);
    for (;;) {
        auto last = match(input.begin(), input.end(), prompt.query());
        auto it = input.cbegin();
        int i = 1;
        for (; it != last; ++i, ++it) {
            string str = (*it).str.substr(0, screen_size.columns);
            if (i == selection) {
                tty.puts_highlighted(str.c_str());
            } else {
                tty.puts(str.c_str());
            }
            if (i == MAXLINES)
                break;
            tty.putchar('\n');
        }
        tty.cursor_up(i);
        tty.move_to_col(prompt.current_column());

        ch = tty.getchar();
        if (ch == Term::ESC || ch == Term::CTRL_C || ch == Term::ENTER) {
            break;
        } else if (ch == Term::CTRL_N) {
            selection = min(selection + 1, i - 1);
        } else if (ch == Term::CTRL_P) {
            selection = max(selection - 1, 1);
        } else {
            if (prompt.handle_key(static_cast<Term::Key>(ch))) {
                selection = 1;
            }
        }
        tty.putchar('\n');
        tty.erase_display(Term::FORWARD);
    }

    tty.move_to_col(1);
    tty.erase_display(Term::FORWARD);

    if (ch == Term::ENTER) {
        cout << input[selection - 1].str.c_str() << endl;
        return 0;
    }
    return 1;
}
