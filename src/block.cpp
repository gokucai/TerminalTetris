#include <unistd.h>
#include "screen.hpp"
#include "block.hpp"

using std::make_shared;


void Base_block::plot(unsigned x, unsigned y, Color c)
{
    size_t sz = matrix.size();
    for (size_t i = 0; i < sz; ++i) {
        save_cursor();
        move_cursor(x + i, y);
        for (size_t j = 0; j < sz; ++j) {
            if (matrix[i][j] == 1)
                print_point(c);
            else
                move_cursor(0, 1);
        }
        retrieve_cursor();
    }
    flush(std::cout);
}

Block::Block(Block_type block_type) : btype(block_type)
{
    switch (block_type) {
        case CUBE:
            bptr = make_shared<Cube_block>(YELLOW);
            break;
        case L:
            bptr = make_shared<L_block>(BLUE);
            break;
        case Z:
            bptr = make_shared<Z_block>(RED);
            break;
        case BAR:
            bptr = make_shared<Bar_block>(MAGENTA);
            break;
        case TRI:
            bptr = make_shared<Tri_block>(GREEN);
            break;
        default:
            std::cerr << "something went wrong in block_type";
            return;
    }
}
