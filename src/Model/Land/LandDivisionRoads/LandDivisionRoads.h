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

    double divisionArea = 30000;
    double externalRoad = 20;
    double centralRoad = 20;
    double circularStreet = 5;
    double landDepth = 20;
    double streetCut = 5; // for circular Streets

    Point getIntersectionPoint(const Point &p1, double slope, const Line &line);
    Point getIntersectionPoint(const Line& line1, const Line& line2, const Point& pointOnLine1);

    vector<pair<Polygon1 , Polygon1>> dividePolygons(Polygon1 polygon1 , double scalingStreet = 1);

    /**
     * Build Outside Land for curLand
     * @param mainLand
     * @return
     */
    Polygon1 buildOutsideRoads(Polygon1 &mainLand);

    Point shiftPointOnLine(const Line& line, const Point& p, double distance);

    double findY(double x, double x1, double y1, double x2, double y2);
    double findX(double y, double x1, double y1, double x2, double y2);

    static int COUNT ;

public:

    /**
     * Get Outer Land
     * @return
     */
    Polygon1 getOuterLand(Polygon1 &mainLand);

    LandDivisionRoads(double divisionArea, double externalRoad, double centralRoad, double circularStreet,
                      double landDepth, double streetCut);

    /**
     * Split Land with Roads Based on number of Divisions
     * @param land
     * @param ratioA
     * @param ratioB
     * @param maxDivisions
     * @return
     */
    virtual vector<vector<Polygon1>> divideLand(const Polygon1 &land , double ratioA , double ratioB , int maxDivisions ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on min Area should each Sub Land Have
     * @param land
     * @param area
     * @return
     */
    virtual vector<vector<Polygon1>> divideLand(const Polygon1 &land , double area ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on Inner Home Design should set inside each Sub Land
     * @param land
     * @param innerHome
     * @return
     */
    virtual vector<vector<Polygon1>> divideLand(const Polygon1 &land , const Polygon1 &innerHome ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on number of Divisions
     * @param land
     * @param ratioA
     * @param ratioB
     * @param maxDivisions
     * @return
     */
    virtual vector<vector<Polygon1>> divideLands(vector< Polygon1> &lands , double ratioA , double ratioB , int maxDivisions,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on min Area should each Sub Land Have
     * @param land
     * @param area
     * @return
     */
    virtual vector<vector<Polygon1>> divideLands(vector< Polygon1> &lands , double area ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on Inner Home Design should set inside each Sub Land
     * @param land
     * @param innerHome
     * @return
     */
    virtual vector<vector<Polygon1>> divideLands(vector< Polygon1> &lands , const Polygon1 &innerHome ,LandDivisionSortingStrategy  landDivisionStrategy  ) {vector<vector<Polygon1>> tst; return tst;}

    /**
     * Split Land Based on list of ratios
     * @param land
     * @param ratios
     * @param landDivisionStrategy
     * @return
     */
    virtual vector<vector<Polygon1>> divideLand(const Polygon1 &land ,const vector<double> &ratios,LandDivisionSortingStrategy  landDivisionStrategy ) {vector<vector<Polygon1>> tst; return tst;};

    /**
     * Split Land Based on list of ratios
     * @param lands
     * @param ratios
     * @param landDivisionStrategy
     * @return
     */
    virtual vector<vector<Polygon1>>divideLands(vector<Polygon1> &lands,const vector<double> &ratios,LandDivisionSortingStrategy  landDivisionStrategy) { vector<vector<Polygon1>> tst; return tst; };
};
#endif //DESIGNI_BACKEND_LANDDIVISIONROADS_H
