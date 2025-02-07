//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_ROOMSHAPESTRATEGY_H
#define DESIGNI_BACKEND_ROOMSHAPESTRATEGY_H
#include "src/Model/Design.h"
class RoomShapeStrategy{
public:
    /**
     *  create Template Shapes
     * @return
     */
    virtual vector<Design> createShapes() = 0;
};
#endif //DESIGNI_BACKEND_ROOMSHAPESTRATEGY_H
