//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_DESIGNVALIDATOR_H
#define DESIGNI_DESIGNVALIDATOR_H
#include "Design.h"
//Responsible for validate Design in the different Side
// 1- aspect ratio between height and width
// 2- all connections are satisfied
// 3- all Views are satisfied
// 4- no room conflicts with any other room
class DesignValidator {
public:
/**
     * Is Point Inside Rooms
     * @param point
     * @param rooms
     * @return
     */
    static bool isPointOutsideRooms(const Point &point , const vector<Room> &rooms);

    /**
     * Is Room Outside Rooms
     * @param newRoom
     * @param rooms
     * @return
     */
    static bool isRoomOutsideRooms(const Room &newRoom, const vector<Room> &rooms);

    /**
    * Is Two Rooms overlapping?
    * @param one
    * @param two
    * @return
    */
    static bool isTwoRoomsIntersect(const Room &one, const Room &two);

    /**
     * Is Two Room Connected Or Not
     * @param one
     * @param two
     * @return
     */
    static bool isTwoRoomsConnected(const Room &one, const Room &two);

    /**
     * Validate Design Overlapped
     * @param rooms
     * @return
     */
    static bool validateDesign(const vector<Room> &rooms);
};


#endif //DESIGNI_DESIGNVALIDATOR_H
