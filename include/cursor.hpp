#ifndef INCLUDE_CURSOR_HDR
#define INCLUDE_CURSOR_HDR

#include <iostream>

enum Color {
    CLEAR,
    BLACK = 40,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
};


//==================================================
// functions for cursor move
//==================================================


// these two functions must be called in pairs
void save_cursor();
void retrieve_cursor();

void move_cursor(size_t, size_t);

//==================================================
// functions for print point
//==================================================

void print_point(Color = CLEAR);

//==================================================
// for print pretty text
//==================================================

const std::string ITALIC = "\033[3m";
const std::string NORMAL = "\033[0m";

#endif
