//
// Created by ahmed Ibrahim on 30-Nov-24.
//

#include "Point.h"

double Point::getX() const {
    return x;
}

void Point::setX(double x) {
    Point::x = x;
}

double Point::getY() const {
    return y;
}

void Point::setY(double y) {
    Point::y = y;
}

double Point::getZ() const {
    return z;
}

void Point::setZ(double z) {
    Point::z = z;
}

Point::Point(double x, double y) : x(x), y(y) {}
Point::Point(double x, double y , double z) : x(x), y(y) , z(z){}

Point::Point() {
    x = y  =0.0; z = 0.0;
}
