//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#ifndef DESIGNI_LANDNETWORKBYSCALING_H
#define DESIGNI_LANDNETWORKBYSCALING_H
#include "LandNetwork.h"

/**
 * Build Land Network By Scale each Land
 */
class LandNetworkByScaling : public LandNetwork{
public:
    vector<Polygon1> buildLandNetwork(const Polygon1 &mainPolygon, const vector<Polygon1> &polys) override;
};


#endif //DESIGNI_LANDNETWORKBYSCALING_H
