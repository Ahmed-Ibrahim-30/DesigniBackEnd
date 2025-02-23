//
// Created by ahmed Ibrahim on 22-Feb-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONROADS_H
#define DESIGNI_BACKEND_LANDDIVISIONROADS_H
#include "src/Model/Polygon.h"
#include "src/Model/Line.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "../SortLandDivision/SortLandDivisionsByMinimizedDimensions.h"
#include "../SortLandDivision/SortLandDivisionsByMinimizedAcuteAngles.h"
#include "../SortLandDivision/SortLandDivisionsByMinimizedConnections.h"
#include "../SortLandDivision/SortLandDivisionsByMaximizedConnections.h"
#include "../SortLandDivision/SortLandDivisionsByMinimizingLengthVariance.h"

class LandDivisionRoads{
protected:
    Point getIntersectionPoint(const Point &p1, double slope, const Line &line);
    Point getIntersectionPoint(const Line& line1, const Line& line2, const Point& pointOnLine1);

    vector<pair<Polygon1 , Polygon1>> dividePolygons(Polygon1 polygon1);

    Point shiftPointOnLine(const Line& line, const Point& p, double distance);

    double findY(double x, double x1, double y1, double x2, double y2);
    double findX(double y, double x1, double y1, double x2, double y2);

    static int COUNT ;

public:
    /**
     * Split Land with Roads Based on number of Divisions
     * @param land
     * @param ratioA
     * @param ratioB
     * @param maxDivisions
     * @return
     */
    virtual vector<vector<Polygon1>> divideLand(const Polygon1 &land , double ratioA , double ratioB , int maxDivisions ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}
};
#endif //DESIGNI_BACKEND_LANDDIVISIONROADS_H
