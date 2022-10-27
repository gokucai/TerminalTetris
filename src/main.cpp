#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "screen.hpp"

using namespace std;

// key code for direction keys
// those values can be found using command 'showkey -a'
#define KEY_LEFT    68
#define KEY_UP      65
#define KEY_RIGHT   67
#define KEY_DOWN    66

// 5e5 us = 500 ms = 0.5 s
const int INTERVAL = 5e5;
pthread_mutex_t mlock;
bool has_generated;

void *key_stroke(void *ptr)
{
    Screen *s = (Screen *)ptr;
    while (true) {
        system("stty -icanon -echo");
        int ch = cin.get();
        system("stty icanon echo");

        pthread_mutex_lock(&mlock);
        switch (ch) {
            case 'a': case KEY_LEFT:
                if (s->is_stopped())
                    break;

                if (!s->check_collision(LEFT))
                    s->move_block(LEFT);
                break;

            case 'w': case KEY_UP:
                s->rotate_block();
                break;

            case 'd': case KEY_RIGHT:
                if (s->is_stopped())
                    break;

                if (!s->check_collision(RIGHT))
                    s->move_block(RIGHT);
                break;

            case 's': case KEY_DOWN:
                while (!s->check_collision(DOWN))
                    s->move_block(DOWN);
                s->eliminate();
                s->generate_block();
                break;

            case 'p':
                s->pause();
                break;

            default:
                break;
        }
        pthread_mutex_unlock(&mlock);
    }
}

int main()
{
    system("clear");
    Screen s;

    pthread_mutex_init(&mlock, nullptr);

    pthread_t pt;
    pthread_create(&pt, nullptr, key_stroke, (void *)(&s));

    while (true) {
        has_generated = false;
        usleep(INTERVAL);
        while (true) {
            if (s.is_stopped())
                continue;

            pthread_mutex_lock(&mlock);
            if (has_generated || s.check_collision(DOWN)) {
                pthread_mutex_unlock(&mlock);
                s.eliminate();
                break;
            }
            s.move_block(DOWN);
            pthread_mutex_unlock(&mlock);

            usleep(INTERVAL);
        }
        if (!has_generated)
            s.generate_block();
    }
}


