#include "key_input.h"
#include <conio.h>

Key get_key() {

    bool finished = false;
    do {
        finished = true;
        char input = _getch();
        if (input == 0x00 || input == 0xe0) {
            input = _getch();
        }
        switch (input) {
        case(0x4b):
            return Key::left;
        case(0x48):
            return Key::up;
        case(0x50):
            return Key::down;
        case(0x4d):
            return Key::right;
        case('O'):
        case('o'):
            return Key::open;
        case('F'):
        case('f'):
            return Key::flag;
        case('Q'):
        case('q'):
            return Key::quit;
        default:
            finished = false;
        }
    } while (!finished);
}

void hit_any_key() {
    _getch();
}