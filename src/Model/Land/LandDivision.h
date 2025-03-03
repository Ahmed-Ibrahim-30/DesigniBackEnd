//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#ifndef DESIGNI_LANDDIVISION_H
#define DESIGNI_LANDDIVISION_H
#include "src/Model/Polygon.h"
#include "src/Model/Line.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "SortLandDivision/SortLandDivisionsByMinimizedDimensions.h"
#include "SortLandDivision/SortLandDivisionsByMinimizedAcuteAngles.h"
#include "SortLandDivision/SortLandDivisionsByMinimizedConnections.h"
#include "SortLandDivision/SortLandDivisionsByMaximizedConnections.h"
#include "SortLandDivision/SortLandDivisionsByMinimizingLengthVariance.h"

/**
 * Split Land To multiple Land
 */
class LandDivision {
protected:
    Point getIntersectionPoint(const Point &p1, double slope, const Line &line);
    Point getIntersectionPoint(const Line& line1, const Line& line2, const Point& pointOnLine1);

    vector<pair<Polygon1 , Polygon1>> dividePolygons(Polygon1 polygon1);

public:
    /**
     * Split Land Based on number of Divisions
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

#endif //DESIGNI_LANDDIVISION_H
