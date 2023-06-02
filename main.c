#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "dos.h"

int size = 3;
int color = 0;
int offset_x = 0, offset_y = 0;

enum mods{
    none,
    rainbow,
    mods_count
} mode = none;

void sqr(int x, int y);
void output(bool map[], int X, int Y);
void logic(bool _map[], bool map[], int X, int Y);

int main()
{
    setvideomode(videomode_640x480);
    const int H = 640, W = 480;
    const int X = H / size, Y = W / size;

    bool map[X][Y], _map[X][Y];
    for(int y = 0; y < Y; y++)
        for(int x = 0; x < X; x++)
            _map[x][y] = map[x][y] = rand() % 2;

    int delay = 50;
    for(int i = 0; !shuttingdown(); i++)
    {
        waitvbl();

        logic(&map[0][0], &_map[0][0], X, Y);
        output(&map[0][0], X, Y);

        mode = (keystate(KEY_SPACE) + mode) % mods_count;

        size += (keystate(KEY_W) != 0) - (keystate(KEY_S) != 0);

        offset_x += ((keystate(KEY_LEFT) != 0) - (keystate(KEY_RIGHT) != 0)) * 3;
        offset_y += ((keystate(KEY_UP) != 0) - (keystate(KEY_DOWN) != 0)) * 3;
        
        for(clock_t start_time = clock(); clock() < start_time + (delay * 1000);){}//remove on windows
    }
    return 0;
}

void sqr(int x, int y)
{
    x += offset_x;
    y += offset_y;
    x *= size;
    y *= size;
    int points[] = 
    {
        x, y,
        x + size, y,
        x + size, y + size,
        x, y + size
    };
    fillpoly(points, 4);
}

void logic(bool _map[], bool map[], int X, int Y)
{
    for(int y = 0; y < Y; y++)
    {
        for(int x = 0; x < X; x++)
        {
            int count_neighbours = -_map[x * Y + y];
            for(int i = -1; i < 2; i++)
                for(int j = -1; j < 2;j++)
                    count_neighbours += _map[(x - i) * Y + y - j];

            map[x * Y + y] = (count_neighbours == 3 || (count_neighbours == 2 && _map[x * Y + y] == 1));
        }
    }
    for(int y = 0; y < Y; y++)
        for(int x = 0; x < X; x++)
            _map[x * Y + y] = map[x * Y + y];
}

void output(bool map[], int X, int Y)
{
    switch (mode)
    {
    case none:
        clearscreen();
        break;
    case rainbow:
        setcolor(color++ % 215 + 16);
    }
    for(int y = 0; y < Y; y++)
        for(int x = 0; x < X; x++)
            if(map[x * Y + y])
                sqr(x, y);
}
