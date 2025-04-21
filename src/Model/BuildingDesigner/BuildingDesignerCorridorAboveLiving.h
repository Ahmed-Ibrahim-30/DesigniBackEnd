//
// Created by ahmed Ibrahim on 10-Apr-25.
//

#ifndef DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORABOVELIVING_H
#define DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORABOVELIVING_H
#include "BuildingDesigner.h"

class BuildingDesignerCorridorAboveLiving :public BuildingDesigner{
public:
    BuildingDesignerCorridorAboveLiving(const vector<string> &zone1Ids, const vector<string> &zone2Ids,
                                         const vector<pair<string, string>> &connections,
                                         map<string, double> &roomsAreas);

    BuildingDesignerCorridorAboveLiving(const vector<string> &zone1Ids, const vector<string> &zone2Ids,
                                         const vector<pair<string, string>> &connections,
                                         map<string, pair<double, double>> &dimensionsLimits);

    vector<Design> generateDesign() override;

protected:
    vector<Room> generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom) override;
    vector<Room> generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom, int corridorCount) override;

};


#endif //DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORABOVELIVING_H
