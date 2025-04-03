//
// Created by ahmed Ibrahim on 03-Apr-25.
//

#ifndef DESIGNI_BACKEND_ROOMENTITY_H
#define DESIGNI_BACKEND_ROOMENTITY_H
#include "src/Model/Room.h"

class RoomEntity : public Room{
private:
    pair<double , double> dimensionLimit;
public:
    RoomEntity(const string &id , const vector<string> &roomBesides ,const pair<double , double> &_dimensionLimit)
    :Room(){
        changeId(id);
        setRoomsBesides(roomBesides);
        dimensionLimit = _dimensionLimit;
    }

    const pair<double, double> &getDimensionLimit() const
    {
        return dimensionLimit;
    }
};
#endif //DESIGNI_BACKEND_ROOMENTITY_H
