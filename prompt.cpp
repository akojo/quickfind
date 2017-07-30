#include <iostream>
#include <iterator>

#include "prompt.h"

Prompt::Prompt(Term& tty, const std::string& prompt)
    : tty{tty}, prompt{prompt}, input{}
{
    tty.puts((prompt + "\n").c_str());
}

bool Prompt::handle_key(const Term::Key key)
{
    bool reset_selection = true;

    if (isprint(key)) {
        input.push_back(key);
    } else if (input.length() > 0) {
        if (key == Term::DEL || key == Term::BACKSPACE) {
            input.pop_back();
        } else if (key == Term::CTRL_U) {
            input.clear();
        }
        reset_selection = false;
    } else {
        reset_selection = false;
    }
    tty.move_to_col(1);
    tty.erase_line(Term::FORWARD);
    tty.puts(prompt + input);
    return reset_selection;
}
