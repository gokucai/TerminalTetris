#ifndef INCLUDE_MENU_HDR
#define INCLUDE_MENU_HDR

#include <array>
#include <memory>
#include "cursor.hpp"
#include "block.hpp"

class Screen;

//==================================================
//
// class Menu
//
//==================================================

class Menu
{
    friend class Screen;

public:
    Menu() = default;
    Menu(unsigned _x, unsigned _y)
        : x(_x), y(_y) { }


protected:
    // show menu on screen
    virtual void show() = 0;

    // coordinate of menu on screen
    unsigned x = 0, y = 0;
};



//==================================================
//
// class Score
//
//==================================================

class Score : public Menu
{
    friend class Screen;
public:
    Score() = default;
    Score(unsigned _x, unsigned _y, unsigned sco)
        : Menu(_x, _y), score(sco) { }

private:
    void show() override;

    unsigned score = 0;
};

inline void Score::show()
{
    save_cursor();

    move_cursor(x, y);
    std::cout << ITALIC << "Score: "
        << score << NORMAL;

    retrieve_cursor();
}



//==================================================
//
// class Pred_next
//
//==================================================

class Pred_next : public Menu
{
    friend class Screen;

public:
    Pred_next() = default;
    Pred_next(unsigned _x, unsigned _y)
        : Menu(_x, _y) { }
    Pred_next(unsigned _x, unsigned _y, const Block &b)
        : Menu(_x, _y), next_block(b) { }

private:
    void show() override;
    void erase_block()
    {
        if (!next_block.empty())
            next_block.erase(x + 2, y + 2);
    }
    void show_block() { next_block.show(x + 2, y + 2); }
    void set_next(const Block &nb) { next_block = nb; }

    Block next_block;
};

inline void Pred_next::show()
{
    save_cursor();
    move_cursor(x, y);
    std::cout << ITALIC << "Next: " << NORMAL;
    retrieve_cursor();

    // next_block.erase(x + 2, y + 2);
    // next_block.show(x + 2, y + 2);
}



//==================================================
//
// class Score
//
//==================================================

class Help_msg : public Menu
{
    friend class Screen;

public:
    Help_msg() = default;
    Help_msg(unsigned _x, unsigned _y) : Menu(_x, _y) { }

private:
    void show() override;

    const std::array<std::string, 5> msgs = {
        "w: rotate the block",
        "a: move to left",
        "s: drop the block",
        "d: move to right",
        "p: pause",
    };
};

inline void Help_msg::show()
{
    save_cursor();
    move_cursor(x, y);
    std::cout << ITALIC << "Help: ";
    retrieve_cursor();

    int i = 1;
    for (const std::string &msg : msgs) {
        save_cursor();
        move_cursor(x + i++, y);
        std::cout << ITALIC << "\t" << msg;
        retrieve_cursor();
    }
    std::cout << NORMAL;
}



#endif
