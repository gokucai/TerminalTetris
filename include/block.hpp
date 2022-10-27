#ifndef INCLUDE_BLOCK_HDR
#define INCLUDE_BLOCK_HDR

#include <array>
#include <memory>
#include "cursor.hpp"


enum Block_type {
    CUBE = 1,
    L,
    Z,
    BAR,
    TRI,
};


class Block;
class Screen;

class Base_block
{
    friend class Screen;
    friend class Block;

public:
    Base_block(Color c) : col(c) { }

protected:
    // just plot the block on specified coordinate
    void show(unsigned x, unsigned y) { plot(x, y, col); }

    // just erase this block on specified coordinate
    void erase(unsigned x, unsigned y) { plot(x, y, CLEAR); }

    Color col;
    std::array<std::array<int, 3>, 3> matrix;

private:
    void plot(unsigned, unsigned, Color);
};



/*
 * an encapsulation of Base_block
 * */
class Block
{
    friend class Screen;

public:
    Block() = default;
    Block(Block_type);

    void show(unsigned x, unsigned y) { bptr->show(x, y); }
    void erase(unsigned x, unsigned y) { bptr->erase(x, y); }
    bool empty() { return bptr.get() == nullptr; }

private:
    std::shared_ptr<Base_block> bptr;
    Block_type btype;
};



class Cube_block : public Base_block
{
public:
    Cube_block(Color c) : Base_block(c)
    {
        matrix = {{
                {1, 1, 0},
                {1, 1, 0},
                {0, 0, 0},
            }
        };
    }

protected:
};



class L_block : public Base_block
{
public:
    L_block(Color c) : Base_block(c)
    {
        matrix = {{
                {1, 0, 0},
                {1, 0, 0},
                {1, 1, 0},
            }
        };
    }
};



class Z_block : public Base_block
{
public:
    Z_block(Color c) : Base_block(c)
    {
        matrix = {{
                {1, 1, 0},
                {0, 1, 1},
                {0, 0, 0},
            }
        };
    }
};



class Bar_block : public Base_block
{
public:
    Bar_block(Color c) : Base_block(c)
    {
        matrix = {{
                {0, 1, 0},
                {0, 1, 0},
                {0, 1, 0},
            }
        };
    }
};



class Tri_block : public Base_block
{
public:
    Tri_block(Color c) : Base_block(c)
    {
        matrix = {{
                {0, 1, 0},
                {1, 1, 1},
                {0, 0, 0},
            }
        };
    }
};

#endif
