//
// Created by ahmed Ibrahim on 07-Apr-25.
//

#include "CorridorDesigner.h"

CorridorDesigner::CorridorDesigner(vector<RoomEntity> &_rooms): rooms(_rooms) {}

const Room &CorridorDesigner::getCorridor() const {
    return corridor;
}

void CorridorDesigner::design() {

}


