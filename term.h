#ifndef TERM_H
#define TERM_H

extern "C" {
#include <stdio.h>
#include <termios.h>
}

class Term
{
public:
    enum Key {
        CTRL_A = 1,
        CTRL_B,
        CTRL_C,
        CTRL_D,
        CTRL_E,
        CTRL_F,
        CTRL_G,
        CTRL_H,
        BACKSPACE = CTRL_H,
        CTRL_I,
        TAB = CTRL_I,
        CTRL_J,
        ENTER = CTRL_J,
        CTRL_K,
        CTRL_L,
        CTRL_M,
        LF = CTRL_M,
        CTRL_N,
        CTRL_O,
        CTRL_P,
        CTRL_Q,
        CTRL_R,
        CTRL_T,
        CTRL_S,
        CTRL_U,
        CTRL_V,
        CTRL_W,
        CTRL_X,
        CTRL_Y,
        CTRL_Z,
        ESC,
        DEL = 127
    };

    enum Direction {
        FORWARD = 0,
        BACKWARDS = 1,
        BOTH = 2
    };

    struct Size {
        int rows;
        int columns;
    };

    Term(const char* ttyName);
    ~Term();

    int puts(const char *str);
    int putchar(int ch);
    int getchar();

    Size get_screen_size();
    void cursor_up(int n);
    void cursor_down(int n);
    void cursor_fwd(int n);
    void cursor_back(int n);
    void move_to_col(int n);
    void erase_display(Direction d = Term::BOTH);
    void erase_line(Direction d = Term::BOTH);

private:
    int fd;
    FILE *tty;
    struct termios old_termios, new_termios;
};

#endif // TERM_H
