//
// Created by ahmed Ibrahim on 15-Feb-25.
//

#ifndef DESIGNI_BACKEND_SORTLANDDIVISIONS_H
#define DESIGNI_BACKEND_SORTLANDDIVISIONS_H
#include "src/Model/Polygon.h"
#include "src/Model/Line.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"

enum LandDivisionSortingStrategy{
    MinimizingLengthVariance = 0,
    MinimizeSmallDimensions,
    MinimizeAcuteAngles,
    MinimizeLandConnections,
    MaximizeLandConnections
};

class SortLandDivisions {
public:

    /**
     * sort Divisions
     * @param polygons
     * @return
     */
    virtual vector<vector<Polygon1>> sortDivisions(vector<vector<Polygon1>> &divisions) = 0;
};


#endif //DESIGNI_BACKEND_SORTLANDDIVISIONS_H
