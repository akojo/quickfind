#include "term.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}

#include <stdexcept>
#include <string>

using namespace std;

Term::Term(const string& ttyName)
{
    tty = fopen(ttyName.c_str(), "r+");
    if (!tty)
        throw invalid_argument("bad tty");
    setbuf(tty, nullptr);

    int fd = fileno(tty);
    if (tcgetattr(fd, &old_termios) < 0)
        throw runtime_error("failed to read terminal parameters");

    struct termios new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    if (tcsetattr(fd, TCSAFLUSH, &new_termios) < 0) {
        throw runtime_error(string("failed to set terminal parameters: ") + strerror(errno));
    }
}

Term::~Term()
{
    tcsetattr(fileno(tty), TCSAFLUSH, &old_termios);
}

int Term::puts(const string& str)
{
    return fputs(str.c_str(), tty);
}

int Term::puts_highlighted(const string& str) {
    return fprintf(tty, "\033[7m%s\033[0m", str.c_str());
}

int Term::putchar(int ch){
    return putc(ch, tty);
}

int Term::getchar()
{
    int i;
    i = getc(tty);
    if (i == 0)
        return -1;
    if (i < 0)
        throw runtime_error("read error");
    return i;
}

Term::Size Term::get_screen_size()
{
    struct winsize ws;
    ioctl(fileno(tty), TIOCGWINSZ, &ws);
    return Size{ws.ws_row, ws.ws_col};
}

void Term::cursor_up(int n)
{
    fprintf(tty, "\033[%dA", n);
}

void Term::cursor_down(int n)
{
    fprintf(tty, "\033[%dB", n);
}

void Term::cursor_fwd(int n)
{
    fprintf(tty, "\033[%dC", n);
}

void Term::cursor_back(int n)
{
    fprintf(tty, "\033[%dD", n);
}

void Term::move_to_col(int n)
{
    fprintf(tty, "\033[%dG", n);
}

void Term::erase_display(Direction d)
{
    fprintf(tty, "\033[%dJ", d);
}

void Term::erase_line(Direction d)
{
    fprintf(tty, "\033[%dK", d);
}
