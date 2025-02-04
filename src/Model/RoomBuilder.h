//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_ROOMBUILDER_H
#define DESIGNI_ROOMBUILDER_H
#include "RoomTypes.h"
#include "Room.h"
#include "src/Utils/UniqueIdGenerator.h"
#include "src/Utils/RoomTypeUtils.h"
#include "RoomDefaults.h"

class RoomBuilder{
private:
    /**
     * Room Area
     */
    double area;

    /**
     * Room Dimensions
     */
    double x1 , y1 , x2 , y2;

    /**
     * Rooms Width and Height
     */
    double width , height;

    /**
     * Room Type
     */
    RoomType roomType;

    /**
     * Room Id
     */
    string roomId;

    /**
     * Rooms required View
     */
    vector<char> requiredView;

    /**
     * set Room Area
     * @param area
     * @param roomType1 for default values if area == -1
     */
    void setArea(double area , RoomType roomType1 = RoomType::Other);

    /**
     * Set Room Width
     * @param width
     */
    void setWidth(double width);

    /**
     * Set Room Height
     * @param height
     */
    void setHeight(double height);

    /**
     * Parse Room Name To Room Type and Set Room Type
     * @param roomName
     */
    void setRoomType(const string &roomName);

    /**
     * Set Room Id
     * @param roomType
     * @param roomName
     */
    void setRoomId( const string &roomName);

    /**
     * Set Room requiredView
     * @param requiredView
     */
    void setRequiredView(const vector<char> &requiredView);

    /**
     * set Room x1 , y1 , x2 , y2
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void setDimensions(double x1 , double y1 , double x2 , double y2);

public:
    /**
     * Create Room Object
     * @param roomType
     * @param roomArea
     * @param view
     * @return
     */
    Room createRoom(const string &roomName , double roomArea ,const vector<char>&view = {});

    /**
     * Create Room Object
     * @param roomType
     * @param w
     * @param h
     * @param view
     * @return
     */
    Room createRoom(const string &roomName , double w , double h ,const vector<char>&view = {});

    /**
     * Create Room Object
     * @param roomType
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param view
     * @return
     */
    Room createRoom(const string &roomName , double x1 , double y1 , double x2 , double y2 ,const vector<char>&view = {});
};


#endif //DESIGNI_ROOMBUILDER_H
