#include <pthread.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "screen.hpp"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;

std::default_random_engine Screen::e(time(0));
std::uniform_int_distribution<unsigned> Screen::u(1, 5);

Screen::Screen() : menu_score(5, 19, 0), menu_pred(7, 19), menu_help(13, 19)
{
    // In the  c++ standard library implemented in my system,
    // the first number generated by engine will always be the same,
    // so we discard it.
    e.discard(1);
    next_block = Block((Block_type)get_random());

    contents.front().fill(1);
    colors.front().fill(CYAN);
    for (size_t i = 1; i < contents.size() - 1; ++i) {
        contents[i].fill(0);
        contents[i].front() = 1;
        contents[i].back() = 1;
        colors[i].fill(CLEAR);
        colors[i].front() = CYAN;
        colors[i].back() = CYAN;
    }
    contents.back().fill(1);
    colors.back().fill(CYAN);

    show_skeleton();
    show_menu_list();
    generate_block();
    flush(cout);
}

/*
 * plot the boundary of the game
 * */
void Screen::show_skeleton()
{
    save_cursor();

    for (size_t i = 0; i < width; ++i)
        print_point(CYAN);
    cout << endl;

    for (size_t i = 1; i < height - 1; ++i) {
        print_point(CYAN);
        for (size_t j = 0; j < width - 2; ++j)
            print_point(CLEAR);
        print_point(CYAN);
        cout << endl;
    }

    for (size_t i = 0; i < width; ++i)
        print_point(CYAN);
    cout << endl;

    retrieve_cursor();
}

void Screen::show_menu_list()
{
    menu_score.show();

    // just print text, don't plot the next block graph
    menu_pred.show();

    menu_help.show();

    flush(cout);
}

/* assign 'next block' to 'current block'
 * generate a new 'next block'
 */
void Screen::generate_block()
{
    curr_x = 1;
    curr_y = 7;

    // add std::shared_ptr<Base_block> counter
    Block tmp = curr_block;
    // if not use tmp, the shaerd_ptr curr_block saved
    // will be deleted after this assignment,
    // but we still need to use it in menu_pred.erase_block()
    curr_block = next_block;
    next_block = Block(static_cast<Block_type>(get_random()));

    // game over
    for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            if (next_block.bptr->matrix[i][j] == 1
                    && contents[curr_x + i][curr_y + j] == 1) {
                move_cursor(contents.size() + 1, 0);
                cout << "Game over!" << endl;
                cout << "\t You final score is: " << menu_score.score << endl;
                exit(0);
            }
        }
    }

    show();

    // erase last block on the panel
    menu_pred.erase_block();
    menu_pred.set_next(next_block);
    // plot new block on the panel
    menu_pred.show_block();
}

void Screen::move_block(Direction direct)
{
    erase();
    switch (direct) {
        case LEFT:
            --curr_y;
            break;
        case RIGHT:
            ++curr_y;
            break;
        case DOWN:
            ++curr_x;
            break;
        case UP:
            throw std::runtime_error("can't move block upward");
        default:
            cerr << "Unknown value of Direction";
            return;
    }
    show();
}

void Screen::rotate_block()
{
    if (curr_block.btype == CUBE)
        return;

    decltype(curr_block.bptr->matrix) tmp;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tmp[3 - 1 - j][i] = curr_block.bptr->matrix[i][j];
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tmp[i][j] == 1 && curr_block.bptr->matrix[i][j] == 0
                && contents[curr_x + i][curr_y + j] == 1)
                return;
        }
    }
    erase();
    curr_block.bptr->matrix = tmp;
    show();
}

bool Screen::check_collision(Direction direct)
{
    switch (direct) {
        case LEFT:
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (curr_block.bptr->matrix[i][j] == 1) {
                        if (contents[curr_x + i][curr_y + j - 1] == 1)
                            return true;
                        else
                            break;
                    }
                }
            }
            break;
        case RIGHT:
            for (int i = 0; i < 3; ++i) {
                for (int j = 2; j >= 0; --j) {
                    if (curr_block.bptr->matrix[i][j] == 1) {
                        if (contents[curr_x + i][curr_y + j + 1] == 1)
                            return true;
                        else
                            break;
                    }
                }
            }
            break;
        case DOWN:
            for (int j = 0; j < 3; ++j) {
                for (int i = 2; i >= 0; --i) {
                    if (curr_block.bptr->matrix[i][j] == 1) {
                        if (contents[curr_x + i + 1][curr_y + j] == 1)
                            return true;
                        else
                            break;
                    }
                }
            }
            break;
        case UP:
            throw std::runtime_error(
                "a block can't collide with up direction");
        default:
            throw std::runtime_error("");
    }
    return false;
}

void Screen::eliminate()
{
    static vector<unsigned> lines_elim;
    static unsigned sz_x = contents.size();
    static unsigned sz_y = contents[0].size();

    // for (unsigned i = 0; i < contents.size(); ++i) {
    //     for (unsigned j = 0; j < contents[0].size(); ++j) {
    //         std::cerr << std::setw(2) << contents[i][j] << " ";
    //     }
    //     std::cerr << endl;
    // }

    // std::cerr << endl;

    // for (unsigned i = 0; i < contents.size(); ++i) {
    //     for (unsigned j = 0; j < contents[0].size(); ++j) {
    //         std::cerr << std::setw(2) << colors[i][j] << " ";
    //     }
    //     std::cerr << endl;
    // }

    for (unsigned i = 0; i < 3 && curr_x + i < sz_x - 1; ++i) {

        bool flag = true;

        for (unsigned j = 1; j < sz_y - 1; ++j) {
            if (contents[curr_x + i][j] == 0) {
                flag = false;
                break;
            }
        }

        if (flag)
            lines_elim.push_back(curr_x + i);
    }

    // std::cerr << endl;
    // for (unsigned i : lines_elim)
    //     std::cerr << i << " ";
    // std::cerr << endl;
    // std::cerr << "==================================================";
    // std::cerr << endl;

    if (lines_elim.empty())
        return;

    menu_score.score += lines_elim.size();
    flow_down(lines_elim);
    repaint();
    menu_score.show();
}


void Screen::show()
{
    curr_block.show(curr_x, curr_y);
    for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            if (curr_block.bptr->matrix[i][j] == 1) {
                contents[curr_x + i][curr_y + j] = 1;
                colors[curr_x + i][curr_y + j] = curr_block.bptr->col;
            }
        }
    }
}

void Screen::erase()
{
    curr_block.erase(curr_x, curr_y);
    for (unsigned i = 0; i < 3; ++i) {
        for (unsigned j = 0; j < 3; ++j) {
            if (curr_block.bptr->matrix[i][j] == 1) {
                contents[curr_x + i][curr_y + j] = 0;
                colors[curr_x + i][curr_y + j] = CLEAR;
            }
        }
    }
}

void Screen::flow_down(vector<unsigned> &lines_elim)
{
    // slow and fast pointer algorithm
    unsigned slow = lines_elim.back();
    lines_elim.pop_back();
    unsigned fast = slow - 1;
    while (!lines_elim.empty() && fast == lines_elim.back()) {
        --fast;
        lines_elim.pop_back();
    }

    for (; slow > 0; --slow) {
        if (fast == 0) {
            std::fill(contents[slow].begin() + 1, contents[slow].end() - 1, 0);
            std::fill(colors[slow].begin() + 1, colors[slow].end() - 1, CLEAR);
        } else {
            std::copy(contents[fast].begin(), contents[fast].end(),
                contents[slow].begin());
            std::copy(colors[fast].begin(), colors[fast].end(),
                colors[slow].begin());

            --fast;
            while (!lines_elim.empty() && fast == lines_elim.back()) {
                --fast;
                lines_elim.pop_back();
            }
        }

    }
}

void Screen::repaint()
{
    for (unsigned i = 0; i < contents.size(); ++i) {
        for (unsigned j = 0; j < contents[0].size(); ++j) {
            save_cursor();
            move_cursor(i, j);
            print_point(colors[i][j]);
            retrieve_cursor();
        }
    }
}

void Screen::pause() {
    if (state == STOP)
        state = RUNNING;
    else
        state = STOP;
}
