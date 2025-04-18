//
// Created by ahmed Ibrahim on 16-Apr-25.
//

#ifndef DESIGNI_BACKEND_BUILDINGSECONDFLOORDESIGN_H
#define DESIGNI_BACKEND_BUILDINGSECONDFLOORDESIGN_H
#include "src/Model/DesignGeometryManager.h"
#include "RoomEntity.h"

class BuildingSecondFloorDesign {
    double dimensionDiffFactor = 1.5;
    map<string , set<string>> connections;

    map<string, pair<double, double>> dimensionsLimits;
    map<string, double> roomsArea;

    vector<RoomEntity> zone1;

    vector<RoomEntity> zone2;


    void getAllSecondFloorDesign(int roomIndex ,Design &mainDesign, vector<Design> &designs , Room &living ,Room &corridor, vector<Room> &newRooms);
public:
    /**
     * Parametrized Constructor
     * @param zone1Ids
     * @param zone2Ids
     * @param connections
     * @param dimensionsLimits
     */
    BuildingSecondFloorDesign(const vector<string>& zone1Ids , const vector<string>& zone2Ids ,const vector<pair<string , string>> &connections , map<string , pair<double , double>> &dimensionsLimits);
    BuildingSecondFloorDesign(const vector<string>& zone1Ids , const vector<string>& zone2Ids ,const vector<pair<string , string>> &connections , map<string , double> &roomsAreas);

    vector<Design> generateDesign(Design &firstFloorDesigns);
};


#endif //DESIGNI_BACKEND_BUILDINGSECONDFLOORDESIGN_H
