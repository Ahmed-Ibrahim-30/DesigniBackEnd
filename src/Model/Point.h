//
// Created by ahmed Ibrahim on 30-Nov-24.
//

#ifndef DESIGNI_POINT_H
#define DESIGNI_POINT_H
#include "RoomTypes.h"



class Point {
private:
    double x , y , z;
    string id;
    double epsoln = 1e-1;
public:
    Point(double x, double y);
    Point(double x, double y , double z);
    Point();

    double getX() const;

    void setX(double x);

    double getY() const;

    void setY(double y);

    double getZ() const;

    void setZ(double z);

    const string &getId() const  {return id;}

    void setId(const string &id) { Point::id = id;}

    bool operator<(const Point &l) const {
        if (abs(x - l.getX()) > epsoln)return x < l.x;
        return y < l.y;
    }

    bool operator!=(const Point &p) const {
        return !(*this == p);
    }

    bool operator==(const Point& p) const {
        return abs(x - p.x) <= epsoln && abs (y - p.y) <= epsoln;
    }
};


#endif //DESIGNI_POINT_H
