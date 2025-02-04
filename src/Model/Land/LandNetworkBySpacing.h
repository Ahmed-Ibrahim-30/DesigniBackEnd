//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#ifndef DESIGNI_LANDNETWORKBYSPACING_H
#define DESIGNI_LANDNETWORKBYSPACING_H
#include "LandNetwork.h"

class LandNetworkBySpacing :public LandNetwork
{

    enum shiftingDirection
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

    map<int , map<Point , bool>> storePointsShared(const vector<Polygon1> &polys);

    map<int , map<Point , LandNetworkBySpacing::shiftingDirection>>setShiftDirection(const vector<Polygon1> &polys);

public:
    vector<Polygon1> buildLandNetwork(const Polygon1 &mainPolygon, const vector<Polygon1> &polys) override;
};


#endif //DESIGNI_LANDNETWORKBYSPACING_H
