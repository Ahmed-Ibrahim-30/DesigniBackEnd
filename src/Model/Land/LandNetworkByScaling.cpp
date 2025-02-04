//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#include "LandNetworkByScaling.h"

vector<Polygon1> LandNetworkByScaling::buildLandNetwork(const Polygon1 &mainPolygon, const vector<Polygon1> &polys) {
    vector<Polygon1> pol = polys;

    double scaleFactorLands = 0.95 , scaleFactorMainLand = 1.1;

    Polygon1 mainPol = mainPolygon;
    vector<Point> points3 =mainPol.scalePolygon(scaleFactorMainLand);
    mainPol.setPoints(points3);

    for(auto & p1 : pol)
    {
        vector<Point> points =p1.scalePolygon(scaleFactorLands);
        p1.setPoints(points);
    }

    pol.push_back(mainPol);

    return pol;
}
