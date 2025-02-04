#ifndef WALL_H
#define WALL_H
#include "string.h"
#include "iostream"
using namespace std ;
struct Wall
{
    double x1 , y1 , z1 , x2 , y2 , z2;
    string color;

    Wall(double _x1 , double _y1 , double _z1 , double _x2 , double _y2 , double _z2 , const string &color_)
    {
        x1 = _x1;
        y1 = _y1;
        z1 = _z1;
        x2 = _x2;
        y2 = _y2;
        z2 = _z2;
        color = color_;
    }
};

#endif // WALL_H
