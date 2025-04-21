//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#ifndef DESIGNI_BACKEND_BUILDINGDESIGNER_H
#define DESIGNI_BACKEND_BUILDINGDESIGNER_H

#include "src/Model/DesignGeometryManager.h"
#include "RoomEntity.h"
class BuildingDesigner{
protected:
    double corridorWidth = 10;
    double corridorHeight = 2;

    double dimensionDiffFactor = 1.5;

    vector<RoomEntity> zone1;

    set<string> visited;

    vector<RoomEntity> zone2;

    map<string , set<string>> connections;

    map<string, pair<double, double>> dimensionsLimits;
    map<string, double> roomsArea;

    vector<RoomEntity> sortZone(vector<RoomEntity> &zone);

    void sortZoneRoomsDFS(RoomEntity &curRoom, vector<RoomEntity> &ans,set<string>&zonesIds, vector<RoomEntity> &zone,map<string, int> &mapRoomIndex);

    pair<double , vector<double>> findClosestSum(double x , const vector<vector<double>> &values, vector<RoomEntity> &roomE);

    virtual vector<Room> generateCorridorLayout(vector<RoomEntity> &roomE , Room &mainRoom) = 0;

    virtual vector<Room> generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom, int corridorCount) = 0;
public:
    /**
     * Parametrized Constructor
     * @param zone1Ids
     * @param zone2Ids
     * @param connections
     * @param dimensionsLimits
     */
    BuildingDesigner(const vector<string>& zone1Ids , const vector<string>& zone2Ids ,const vector<pair<string , string>> &connections , map<string , pair<double , double>> &dimensionsLimits);
    BuildingDesigner(const vector<string>& zone1Ids , const vector<string>& zone2Ids ,const vector<pair<string , string>> &connections , map<string , double> &roomsAreas);
    /**
     * Generate Design
     * @return
     */
    virtual vector<Design> generateDesign() = 0;
};


#endif //DESIGNI_BACKEND_BUILDINGDESIGNER_H
