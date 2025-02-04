//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#include "DesignValidator.h"

bool DesignValidator::isPointOutsideRooms(const Point &point, const vector<Room> &rooms)
{
    double x = point.getX() , y = point.getY();
    for(auto &room : rooms)
    {
        if(x > room.getX1() && x < room.getX2() &&
           y > room.getY1() && y < room.getY2()) return false;


    }
    return true;
}
bool DesignValidator::isRoomOutsideRooms(const Room &newRoom, const vector<Room> &rooms)
{
    for(auto &room : rooms)
    {
        if(newRoom.getRoomId() == room.getRoomId())continue;
        if(isTwoRoomsIntersect(newRoom , room))return false;
    }
    return true;
}

bool DesignValidator::isTwoRoomsIntersect(const Room &one, const Room &two) {
    // If one rectangle is on left side of other
    if (one.getX1() >= two.getX2() || two.getX1() >= one.getX2())
        return false;

    // If one rectangle is above other
    if (one.getY1() >= two.getY2() || two.getY1() >= one.getY2())
        return false;

    return true;
}

bool DesignValidator::isTwoRoomsConnected(const Room &one, const Room &two)
{
    double x1 = one.getX1(), x2 = one.getX2(), y1 = one.getY1(), y2 = one.getY2();
    double tx1 = two.getX1(), tx2 = two.getX2(), ty1 = two.getY1(), ty2 = two.getY2();

    // Check vertical adjacency
    if ((tx1 == x2 || tx2 == x1) && ((ty1 < y2 && ty1 >= y1) || (ty2 > y1 && ty2 <= y2) || (ty1 <= y1 && ty2 >= y2))) {
        return true;
    }

    // Check horizontal adjacency
    if ((ty1 == y2 || ty2 == y1) && ((tx1 < x2 && tx1 >= x1) || (tx2 > x1 && tx2 <= x2) || (tx1 <= x1 && tx2 >= x2))) {
        return true;
    }
    return false;
}

bool DesignValidator::validateDesign(const vector<Room> &rooms)
{
    bool isShapeValid = true;
    for(auto &room : rooms)
    {
        if(!isRoomOutsideRooms(room , rooms))
        {
            isShapeValid = false;
            break;
        }
    }
    return isShapeValid;
}
