#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "dos.h"

int size = 3;

void sqr(int x, int y)
{
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
            int count_neighbours = 
            _map[(x - 1) * Y + y - 1] + _map[x * Y + y - 1] + _map[(x + 1) * Y + y - 1] +
            _map[(x - 1) * Y + y] +                   _map[(x + 1) * Y + y] +
            _map[(x - 1) * Y + y + 1] + _map[x * Y + y + 1] + _map[(x + 1) * Y + y + 1];

            map[x * Y + y] = (count_neighbours == 3 || (count_neighbours == 2 && _map[x * Y + y] == 1));

        }
    }
    for(int y = 0; y < Y; y++)
    {
        for(int x = 0; x < X; x++)
        {
            _map[x * Y + y] = map[x * Y + y];
        }
    }
}

void output(bool map[], int X, int Y)
{
    clearscreen();
    for(int y = 0; y < Y; y++)
    {
        for(int x = 0; x < X; x++)
        {
            if(map[x * Y + y])
            {
                sqr(x, y);
            }
        }
    }
}


int main()
{
    setvideomode(videomode_640x480);
    int H = 640, W = 480;
    const int X = H / size, Y = W / size;

    bool map[X][Y], _map[X][Y];

    int delay = 50;
    int color = 0;

    for(int y = 0; y < Y; y++)
    {
        for(int x = 0; x < X; x++)
        {
            _map[x][y] = map[x][y] = rand() % 2;
        }
    }

    setcolor(100);
    for(int i = 0; !shuttingdown(); i++)
    {
        waitvbl();

        logic(&map[0][0], &_map[0][0], X, Y);
        output(&map[0][0], X, Y);
        
        for(clock_t start_time = clock(); clock() < start_time + (delay * 1000);){}
    }
    return 0;
}