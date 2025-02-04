//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#ifndef DESIGNI_LANDNETWORK_H
#define DESIGNI_LANDNETWORK_H
#include "src/Model/Polygon.h"

class LandNetwork{
public:

    /**
     * is Point on Line or not
     * @param p
     * @param line
     * @return
     */
    static bool isPointInSegment(const Point &p, const Line &line);

    /**
     * Get Slope
     * @param x1
     * @param x2
     * @param y1
     * @param y2
     * @return
     */
    static double getSlope(double x1, double x2, double y1, double y2);
public:
    /**
     * Build Roads between Land Polygons
     * @param mainPolygon
     * @param polys
     */
    virtual vector<Polygon1> buildLandNetwork(const Polygon1 &mainPolygon ,const vector<Polygon1> &polys) = 0;
};


#endif //DESIGNI_LANDNETWORK_H
