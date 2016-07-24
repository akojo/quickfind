#include <string>
#include "term.h"

class Prompt
{
public:
    Prompt(Term& tty, const std::string& prompt);

    bool handle_key(const Term::Key key);
    int current_column();
    const std::string& query();

private:
    Term tty;
    std::string prompt;
    std::string search_query;
    std::string::iterator pos;
};
