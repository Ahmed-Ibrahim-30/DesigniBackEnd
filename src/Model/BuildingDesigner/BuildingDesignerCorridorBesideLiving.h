//
// Created by ahmed Ibrahim on 10-Apr-25.
//

#ifndef DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORBESIDELIVING_H
#define DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORBESIDELIVING_H
#include "BuildingDesigner.h"

class BuildingDesignerCorridorBesideLiving :public BuildingDesigner{
public:
    BuildingDesignerCorridorBesideLiving(const vector<string> &zone1Ids, const vector<string> &zone2Ids,
                                         const vector<pair<string, string>> &connections,
                                         map<string, double> &roomsAreas);

    BuildingDesignerCorridorBesideLiving(const vector<string> &zone1Ids, const vector<string> &zone2Ids,
                                         const vector<pair<string, string>> &connections,
                                         map<string, pair<double, double>> &dimensionsLimits);

    vector<Design> generateDesign() override;

protected:
    vector<Room> generateCorridorLayout(vector<RoomEntity> &roomE, Room &mainRoom) override;
    vector<Room> generateLivingLayout(vector<RoomEntity> &roomE, Room &mainRoom, int corridorCount) override;
    vector<Design> generateDiffDesign();

    int fetchSuitableLivingWidth(vector<RoomEntity> &livingRooms);

    int fetchSuitableLivingWidth(int roomIndex, int topWidth, int bottomWidth, vector<RoomEntity> &livingRooms);
};


#endif //DESIGNI_BACKEND_BUILDINGDESIGNERCORRIDORBESIDELIVING_H
