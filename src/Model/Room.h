//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_ROOM_H
#define DESIGNI_ROOM_H
#include "RoomTypes.h"
#include "src/Utils/Logging.h"
#include "RoomDefaults.h"
#include "src/Utils/UniqueIdGenerator.h"
#include "Window.h"
#include "Door.h"
#include "Opening.h"

class Room{
private:
    double area;
    double x1 , y1 , x2 , y2;
    /**
     * For 3d Rooms
     */
    double z1 , z2;
    double width , height;
    RoomType roomType;
    string roomId;
    vector<char> requiredView;

    /**
     * Image For Furniture If Founded
     */
    string imagePath;
    /**
     * Room Windows
     */
    vector<Window>windows;

    /**
     * Room Doors
     */
    vector<Door>doors;

    /**
     * Room Openings
     */
    vector<Opening>openings;

    /**
     * Connected Rooms
     */
    vector<string> roomsBesides;

    /**
     * Is Room Internal or external or Top
     */
    string surfaceSide;


    Room(RoomType roomType , const string &roomId , double area,double width, double height, double x1, double y1, double x2, double y2,
         const vector<char> &requiredView = {});
public:
    Room(const string &id1, double x1,double y1,double x2,double y2 , double z1 , double z2)
    {
        changeId(id1);

        setX1(x1);
        setX2(y1);
        setY1(x2);
        setY2(y2);
        setZ1(z1);
        setZ2(z2);
        imagePath = "";
    }

    Room(const string &id1, double x1,double y1,double x2,double y2 )
    {
        changeId(id1);
        setX1(x1);
        setX2(x2);
        setY1(y1);
        setY2(y2);
        setWidth(x2 - x1);
        setHeight(y2 - y1);
        changeArea(getWidth() * getHeight());
        imagePath = "";
    }

    Room(RoomType roomType , double x1,double y1,double x2,double y2,int roomCounter=1,const string &_imagePath = "")
    {
        updateRoomType(roomType);
        changeId(roomId + to_string(roomCounter));
        setX1(x1);
        setX2(x2);
        setY1(y1);
        setY2(y2);
        setWidth(x2 - x1);
        setHeight(y2 - y1);
        changeArea(getWidth() * getHeight());
        setImagePath(_imagePath);
    }

    /**
     * Empty Constructor
     */
    Room() = default;

    friend class RoomBuilder;

    /*
     * Change Id
     */
    void changeId(const string &newId);

    /**
     * Change Room Area
     * @param newArea
     */
    void changeArea(double newArea);

    /**
     * Update Room Type
     * @param roomName
     */
    void updateRoomType(const string &roomName);

    /**
     * Print All Room Details
     */
    void printDetails() const;

    /**
     * Scale Room based on Unit
     * @param unit
     */
    void scaleRoom(double unit = 100);

    /**
     * Rotate Room
     * @param angle
     */
    void rotate(double angle);

    /**
     * Move Room From point to another Point
     * @param xChanged
     * @param yChanged
     */
    void moveRoom(double xChanged, double yChanged);

    /**
     * Add new Window
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     */
    void addWindow(double x1,double y1,double x2,double y2);

    /**
     * Add new Door
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     */
    void addDoor(double x1,double y1,double x2,double y2);

    /**
     * Add new Opening
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     */
    void addOpening(double x1,double y1,double x2,double y2);

    /**
     * Shift Room on X axis form 5 to 10 for example (Right Or Left)
     * @param dx
     */
    void shiftX(double dx);

    /**
     * Shift Room on Y axis form 5 to 10 for example (Top Or Bottom)
     * @param dy
     */
    void shiftY(double dy);

    /**
     * Is Point Inside Room
     * @param p
     * @return
     */
    bool isPointInsideRoom(const Point &p) const;

     vector<Window> &getWindows();

    vector<Window> getWindows1() const;

     vector<Door> &getDoors() ;

     vector<Opening> &getOpenings() ;

    void setX1(double x1);

    void setY1(double y1);

    void setX2(double x2);

    void setY2(double y2);

    double getArea() const;

    double getX1() const;

    const vector<string> &getRoomsBesides() const;

    void setRoomsBesides(const vector<string> &roomsBesides);

    const string &getSurfaceSide() const;

    void setSurfaceSide(const string &surfaceSide);

    double getY1() const;

    double getX2() const;

    double getY2() const;

    double getWidth() const;

    double getHeight() const;

    RoomType getRoomType() const;

    const string &getRoomId() const;

    const vector<char> &getRequiredView() const;

    void setWidth(double width);

    void setHeight(double height);

    /**
     * set Room Dimensions
     * @param _width
     * @param _height
     * @param _x1
     * @param _y1
     */
    void setRoomDimensions(double _width, double _height, double _x1, double _y1);

    void mirrorX();

    void mirrorY();

    void updateRoomType(RoomType newRoomType);

    double getZ1() const;

    void setZ1(double z1);

    double getZ2() const;

    void setZ2(double z2);

    const string &getImagePath() const;

    void setImagePath(const string &imagePath);
};


#endif //DESIGNI_ROOM_H
