//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_ROOMCONNECTIONMANAGER_H
#define DESIGNI_ROOMCONNECTIONMANAGER_H

#include "Design.h"
#include "DesignValidator.h"
//Responsible for connect rooms with each other using cores
class RoomConnectionManager {
private:
    Design currentDesign;

    /**
     * room Level (C , 1 , 2 , 3 , 4 , ..)
     */
    map<string , int> roomLevel;

    /**
     * Rooms that associated to most number of Connections
     */
    set< string > coreRooms;

    /**
     * Room Connection Set
     */
    map<string , set<string>> roomConnection;

    /**
     * each level contains some rooms
     */
    vector<vector<string>>levels;

    /**
     * Map roomsID with Layer
     */
    map<string, int> roomsIDSLayer;

    /**
     * Map roomsID with room Area
     */
    map<string, double> roomsIDSArea;

    /**
     * Object from Room Builder to build any Room
     */
    RoomBuilder roomBuilder;

    /**
     * calculate Level for each room (recursive function)
     * fill roomLevel
     * @param id
     */
    void calculateRoomLevel(const string &id);

    /**
     * Get Corner Points on the all directions
     * @param mainRoom
     * @param rooms
     * @return
     */
    vector<Room> setRoomInCorners(Room mainRoom ,Room requiredRoom,const vector<Room>&rooms);

    /**
     * Store Core Templates Shapes
     * @return
     */
    vector<Design> storeCoreTemplateShapes();

    /**
     * connect Layers To Cores Shapes
     * @param level
     * @param roomIndex
     * @param coreShape
     * @param levelRoomConnections
     * @param roomsCollections
     */
    void connectLayersToCore(int level, int roomIndex, Design &coreShape,
                             const map<string, vector<string>> &levelRoomConnections, vector<Design> &roomsCollections);
public:
    /**
     * Connect Rooms
     * @param design
     * @param cores
     * @return
     */
    virtual vector<Design> connectRooms(const Design &design ,const vector<string> &cores);

    /**
     * connectRooms With Corridor
     * @param design
     * @param cores
     * @param corridorDesigns
     * @return
     */
    virtual vector<Design> connectRooms(const Design &design ,const vector<string> &cores , vector<Design> &corridorDesigns);
};


#endif //DESIGNI_ROOMCONNECTIONMANAGER_H
