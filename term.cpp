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

Term::Term(const char* ttyName)
{
    tty = fopen(ttyName, "r+");
    if (!tty)
        throw invalid_argument("bad tty");
    setbuf(tty, nullptr);

    fd = fileno(tty);
    if (tcgetattr(fd, &old_termios) < 0)
        throw runtime_error("failed to read terminal parameters");

    new_termios = old_termios;
    new_termios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    if (tcsetattr(fd, TCSAFLUSH, &new_termios) < 0) {
        throw runtime_error(string("failed to set terminal parameters: ") + strerror(errno));
    }
}

Term::~Term()
{
    tcsetattr(fd, TCSAFLUSH, &old_termios);
    fclose(tty);
}

int Term::puts(const char *str)
{
    return fprintf(tty, "%s", str);
}

int Term::puts_highlighted(const char *str) {
    return fprintf(tty, "\033[7m%s\033[0m", str);
}

int Term::putchar(int ch){
    return fputc(ch, tty);
}

int Term::getchar()
{
    int i;
    char c;
    i = read(fd, &c, 1);
    if (i == 0)
        return -1;
    if (i < 0)
        throw runtime_error("read error");
    return static_cast<int>(c);
}

Term::Size Term::get_screen_size()
{
    struct winsize ws;
    ioctl(fd, TIOCGWINSZ, &ws);
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
