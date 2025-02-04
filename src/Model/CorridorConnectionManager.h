//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_CORRIDORCONNECTIONMANAGER_H
#define DESIGNI_CORRIDORCONNECTIONMANAGER_H
#include "RoomConnectionManager.h"
#include "AreaToDimensionsCalculator.h"
#include "HorizontalCorridorDimensionsCalculator.h"
#include "TopAlignedRoomCorridorDimensionsCalculator.h"

class CorridorConnectionManager{
private:
    /**
     * Object From corridor Dimensions Calculator
     */
    CorridorDimensionsCalculator * corridorDimensionsCalculator;

    /**
     * connect Shared Room By Corridor
     * @param room
     * @param room1
     * @param sides
     * @param designs
     * @return
     */
    vector<Design> connectSharedRoomByCorridor(const Room &room, const Room &room1, Side &sides, vector<Design> &designs);
public:
    vector<Design> connectCorridors(const Design &design);

    vector<Design>
    connectSharedRoomByCorridorTop(const Room &sharedRoom, const Room &corridor, Side &side, vector<Design> &designs);
};


#endif //DESIGNI_CORRIDORCONNECTIONMANAGER_H
