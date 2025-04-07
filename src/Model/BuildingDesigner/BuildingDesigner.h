//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#ifndef DESIGNI_BACKEND_BUILDINGDESIGNER_H
#define DESIGNI_BACKEND_BUILDINGDESIGNER_H

#include "src/Model/DesignGeometryManager.h"
#include "RoomEntity.h"
class BuildingDesigner{
private:
    double corridorWidth = 10;
    double corridorHeight = 2;

    double dimensionDiffFactor = 1.5;

    vector<RoomEntity> zone1;

    set<string> visited;

    vector<RoomEntity> zone2;

    map<string , set<string>> connections;

    map<string, pair<double, double>> dimensionsLimits;

    vector<RoomEntity> sortZone(vector<RoomEntity> &zone);

    void sortZoneRoomsDFS(RoomEntity &curRoom, vector<RoomEntity> &ans,set<string>&zonesIds, vector<RoomEntity> &zone,map<string, int> &mapRoomIndex);

    vector<Room> generateCorridorLayout(vector<RoomEntity> &roomE , Room &mainRoom);

    static pair<double , vector<double>> findClosestSum(double x , const vector<vector<double>> &values);

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
    Design generateDesign();

    vector<Room> generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom);
};


#endif //DESIGNI_BACKEND_BUILDINGDESIGNER_H
