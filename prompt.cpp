#include "prompt.h"
#include <iostream>
#include <iterator>

Prompt::Prompt(Term& tty, const std::string& prompt)
    : tty{tty}, prompt{prompt}
      
{
    pos = search_query.begin();
    tty.puts((prompt + "\n").c_str());
}
bool Prompt::handle_key(const Term::Key key)
{
    bool reset_selection = false;

    switch (key) {
    case Term::DEL:
    case Term::BACKSPACE:
        if (search_query.length() > 0 && pos != search_query.begin()) {
            pos = std::prev(search_query.erase(pos));
            tty.cursor_back(1);
            tty.erase_line(Term::FORWARD);
            reset_selection = true;
        }
        break;

    case Term::CTRL_U:
        search_query.clear();
        pos = search_query.begin();
        tty.move_to_col(1);
        tty.puts(prompt.c_str());
        reset_selection = true;
        break;

    default:
        pos = std::next(search_query.insert(pos, key));
        tty.putchar(key);
        reset_selection = true;
    }

    tty.erase_display(Term::FORWARD);

    return reset_selection;
}

int Prompt::current_column() {
    return prompt.length() + std::distance(search_query.begin(), pos) + 1;
}

const std::string& Prompt::query() {
    return search_query;
}
