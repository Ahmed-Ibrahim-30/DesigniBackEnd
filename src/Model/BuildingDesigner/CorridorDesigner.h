//
// Created by ahmed Ibrahim on 07-Apr-25.
//

#ifndef DESIGNI_BACKEND_CORRIDORDESIGNER_H
#define DESIGNI_BACKEND_CORRIDORDESIGNER_H
#include "src/Model/DesignGeometryManager.h"
#include "RoomEntity.h"

class CorridorDesigner {
private:
    Room corridor;
    vector<RoomEntity> rooms;
public:
    /**
     * Parametrized Constructor
     * @param rooms : Rooms that must be surrounding to Corridor
     */
    explicit CorridorDesigner(vector<RoomEntity> &rooms);

    /**
     * Design Rooms and Select appropriate Width
     */
    void design();

    const Room &getCorridor() const;
};


#endif //DESIGNI_BACKEND_CORRIDORDESIGNER_H
