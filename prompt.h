#include <string>
#include "term.h"

class Prompt
{
public:
    Prompt(Term& tty, const std::string& prompt);

    bool handle_key(const Term::Key key);
    const int current_column() { return prompt.length() + input.length() + 1; }
    const std::string query() { return input; }

private:
    Term tty;
    std::string prompt;
    std::string input;
};
