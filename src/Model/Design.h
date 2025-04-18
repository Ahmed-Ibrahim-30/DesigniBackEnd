//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_DESIGN_H
#define DESIGNI_DESIGN_H
#include "Room.h"
#include "RoomBuilder.h"
#include "Point.h"
#include "Wall.h"

class Design {
private:
    /**
     * Rooms
     */
    vector<Room> rooms;

    /**
     * Map each Room Id to corresponding Index in rooms
     */
    map<string , int> mapIdToIndex;

    /**
     * All Rooms Connections
     */
    map<string , set<string>> connections;

    /**
     * Map each room with it faces
     */
    map<string , char > roomsIDSFaces;

    /**
     * Map roomsID with Layer
     */
    map<string, int> roomsIDSLayer;

    /**
     * Room Builder to build any Room
     */
    RoomBuilder roomBuilder;

    /**
     * Number Of all Rooms
     */
    int spacesCount;

    /**
     * Number Of Bedrooms
     */
    int bedroomsCount;

    /**
     * Number Of Room (Kitchen and Bath Not Room)
     */
    int roomsCount;

    /**
     * Design Area
     */
    double area;

    /**
     *
     */
    Room mainDesign;

    /**
     * rotatedDegree
     */
    double rotatedDegree;

    double minArea = 0.0;
    double maxArea = 0.0;

    /**
     * Design id
     */
    string designId;
public:
    /**
     * Empty Constructor
     */
    Design();

    /**
     *
     * @param myId
     * @param _rooms
     * @param roomsNumber
     * @param bedroomsNumber
     * @param currentArea
     * @param minArea
     * @param maxArea
     */
    Design(const string &myId ,const vector<Room>&_rooms,int roomsNumber , int bedroomsNumber , double currentArea , double minArea , double maxArea);


    Design(const string &myId ,const vector<Room>&_rooms);

    /**
     * Add Room To Design
     * @param room
     */
    void addRoom(const Room &room);

    /**
     * Delete Room From Design
     * @param room
     */
    void deleteRoom(const Room &room);

    /**
     * Delete Room From Design
     * @param room
     */
    void deleteRoom(const string &roomId);

    /**
     * Scale Design
     * @param unit
     */
    void scaleDesign(double unit = 100);

    /**
     * Move Design from Point to another Point
     * @param xChanged
     * @param yChanged
     */
    void moveDesign(double xChanged, double yChanged);

    /**
     * Move Design from Point to another Point
     * @param newPoint
     */
    void moveDesign(const Point &newPoint);

    /**
     * Move Design from Point to another Point
     * @param newPoint
     */
    void moveDesign(const Point &newPoint , const Point &basePoint);

    /**
     * Rotate Design
     * @param angle
     */
    void rotate(double angle);

    /**
     * Add Connection
     * @param id1
     * @param id2
     */
    void addConnection(const string &id1 , const string &id2);

    /**
     * Change Room Area
     * @param id
     * @param newArea
     */
    void changeRoomArea(const string &id , double newArea);

    /**
     * change Room Name
     */
    void changeRoomName(const string &oldId , const string & newName);

    /**
     * change Room Name
     */
    void changeRoomName(RoomType roomType , const string & newName);

    void changeRoomName(RoomType roomType , RoomType roomType2 , int previousRoomCnt = 1 , int newRoomCnt = 1);

    double getRotatedDegree() const;

    void setRotatedDegree(double rotatedDegree);

    /**
     * Update Main Design after added new Room
     * Called when I want to scale Design Only
     * @param newAddedRoom
     */
    void updateMainDesign(const Room &newAddedRoom);

    /**
     * Fetch Room By Id
     * @param roomId
     * @return
     */
    Room fetchRoomById(const string &roomId) ;

    /**
     * @brief shiftDesignX
     * @param xUnits
     */
    void shiftDesignX(double xUnits);

    /**
     * @brief shiftDesignX
     * @param xUnits
     */
    void shiftDesignY(double yUnits);

    /**
     * Clear All Rooms
     */
    void clearRoom();

    /**
     *
     * @param rooms
     */
    void setRooms(const vector<Room> &rooms);

    /**
     * Update Room Dimension
     * @param room
     */
    void updateRoomDimensions(const string &room , double width , double height);

    /**
     * Update Room Dimension
     * @param room
     */
    void updateRoomDimensions(const string &room , double x1 , double y1 , double x2 , double y2);

    /**
     * initialize Main Design
     */
    void initializeMainDesign();

    /**
     * updateMainDesign
     */
    void updateMainDesign();

    /**
     * addWindows
     */
    void addWindows();

    /**
     * addDoors
     */
    void addDoors();

    /**
     * add Openings
     */
    void addOpenings();

    /**
     * addFurnitureImage
     * @param roomType
     * @param roomCnt
     * @param imagePath
     */
    void addFurnitureImage(RoomType roomType , int roomCnt = 1, const string &imagePath = "");

    /**
     * Add Window to Specific Room
     * @param roomType
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param roomCnt
     */
    void addWindow(RoomType roomType,double x1,double y1,double x2,double y2 , int roomCnt = 1);

    /**
     * Add Door To Specific Room
     * @param roomType
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param roomCnt
     */
    void addDoor(RoomType roomType,double x1,double y1,double x2,double y2 , int roomCnt = 1);

    /**
     * Add Opening To Specific Room
     * @param roomType
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param roomCnt
     */
    void addOpening(RoomType roomType,double x1,double y1,double x2,double y2 , int roomCnt = 1);
    /**
     * @brief getWallFraming
     * @return
     */
    vector<Wall> getWallFraming();

    Room &getMainDesign() ;

    int getSpacesCount() const;

    void setSpacesCount(int spacesCount);

    int getBedroomsCount() const;

    void setBedroomsCount(int bedroomsCount);

    int getRoomsCount() const;

    void setRoomsCount(int roomsCount);

    double getArea() const;

    void setArea(double area);

    const vector<Room> &getRooms() const;

    vector<Room> &getRoomsRef();

    const map<string, set<string>> &getConnections() const;

    void mirrorRecY();

    void mirrorRecX();

    void setConnections(const map<string, set<string>> &connections);

    const map<string, int> &getRoomsIdsLayer() const;

    void setRoomsIdsLayer(const map<string, int> &roomsIdsLayer);

    const map<string, char> &getRoomsIdsFaces() const;

    void setRoomsIdsFaces(const map<string, char> &roomsIdsFaces);

    double getMinArea() const;

    void setMinArea(double minArea);

    double getMaxArea() const;

    void setMaxArea(double maxArea);

    const string &getId() const;

    void setId(const string &id);

};


#endif //DESIGNI_DESIGN_H
