#include <iostream>
#include <string>
#include "cursor.hpp"

void save_cursor()
{
    std::cout << "\033[s";
}

void retrieve_cursor()
{
    std::cout << "\033[u";
}

// move cursor relatively
void move_cursor(size_t x, size_t y)
{
    if (x > 0)
        std::cout << "\033[" + std::to_string(x) + "B";
    if (y > 0)
        std::cout << "\033[" + std::to_string(y << 1) + "C";
}

void print_point(Color col)
{
    std::string ansi_escape;
    if (col == CLEAR)
        ansi_escape = "\033[8m";
    else {
        ansi_escape = "\033[1;" + std::to_string(col) + "m";
    }
    std::cout << ansi_escape << "  \033[0m";

    // switch (col) {
    //     case CLEAR:   std::cout << "\033[8m  \033[0m"; break;
    //     case BLACK:   std::cout << "\033[1;30m  \033[0m"; break;
    //     case RED:     std::cout << "\033[1;31m  \033[0m"; break;
    //     case GREEN:   std::cout << "\033[1;32m  \033[0m"; break;
    //     case YELLOW:  std::cout << "\033[1;m  \033[0m"; break;
    //     case BLUE:    std::cout << "\033[8m  \033[0m"; break;
    //     case MAGENTA: std::cout << "\033[8m  \033[0m"; break;
    //     case CYAN:    std::cout << "\033[8m  \033[0m"; break;
    //     case WHITE:   std::cout << "\033[8m  \033[0m"; break;
    // }
}
