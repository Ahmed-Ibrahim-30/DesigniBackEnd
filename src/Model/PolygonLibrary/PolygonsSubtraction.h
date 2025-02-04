//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#ifndef DESIGNI_POLYGONSSUBTRACTION_H
#define DESIGNI_POLYGONSSUBTRACTION_H
#include "PolygonHelper.h"
#include "LineToPolygonConverter.h"
/**
 * Class Responsible for subtract Polygon from Polygon and return new Polygons

 */
class PolygonsSubtraction{
private:
    Polygon1 mainPolygon;
    Polygon1 secondPolygon;
public:
    PolygonsSubtraction(const Polygon1 &mainPolygon, const Polygon1 &secondPolygon);

    /**
     * return new Polygons
     * @return
     */
    vector<Polygon1> getNewPolygons();
};


#endif //DESIGNI_POLYGONSSUBTRACTION_H
