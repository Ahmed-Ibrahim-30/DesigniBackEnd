//
// Created by ahmed Ibrahim on 14-Jan-25.
//

#ifndef DESIGNI_LAND_H
#define DESIGNI_LAND_H
#include "src/Model/Polygon.h"
#include "LandDivisionBasedOnSidesConvergence.h"
#include "LandDivisionBasedOnLandArea.h"
#include "src/Model/DesignGeometryManager.h"
#include "LandNetworkByScaling.h"
#include "LandNetworkBySpacing.h"
#include "LandDivisionBasedOnInnerDesign.h"
#include "src/Model/DesignOutlines.h"
#include "src/Model/Land/LandDivisionRoads/LandDivisionRoadsByDivisionsCount.h"

#include <cmath>
# define M_PI           3.14159265358979323846

/**
 * Class is Responsible of take Polygon Land and Divide this LandBased On User Demands
 * Set Home Design Inside Any SubLand
 */
class Land {
private:
    Polygon1 land;
    /**
     * SubLand
     */
    vector<Polygon1> subLand;

    /**
     * Pointer to Land Division
     */
    LandDivision * landDivision;

    /**
     * Pointer to Land Division Roads
     */
    LandDivisionRoads * landDivisionRoads;

    /**
     * Pointer to Land Network to create Roads between SubLands
     */
    LandNetwork * landNetwork;
public:
    /**
     * Parameterized Constructor
     * @param land
     */
    explicit Land(const Polygon1 &land);

    /**
     * Parameterized Constructor
     * @param land
     */
    explicit Land() = default;

    /**
     *
     * @param minSubArea
     */
    vector<Polygon1> SplitLand(double minSubArea,LandDivisionSortingStrategy  landDivisionStrategy = LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     *
     * @param design
     */
    vector<Polygon1> SplitLand(const Design &design, LandDivisionSortingStrategy landDivisionStrategy= LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     * Split Land Based On Max Divisions
     * @param divisions
     * @param ratioA
     * @param ratioB
     */
    vector<Polygon1> SplitLand(int divisions , int ratioA , int ratioB, LandDivisionSortingStrategy landDivisionStrategy= LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     *
     * @param minSubArea
     */
    vector<Polygon1> SplitLands(vector<Polygon1> &pols ,double minSubArea, LandDivisionSortingStrategy landDivisionStrategy= LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     *
     * @param design
     */
    vector<Polygon1> SplitLands(vector<Polygon1> &pols ,const Design &design, LandDivisionSortingStrategy landDivisionStrategy= LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     * Split Land Based On Max Divisions
     * @param divisions
     * @param ratioA
     * @param ratioB
     */
    vector<Polygon1> SplitLands(vector<Polygon1> &pols ,int divisions , int ratioA , int ratioB, LandDivisionSortingStrategy landDivisionStrategy= LandDivisionSortingStrategy::MinimizingLengthVariance);

    /**
     * Build Roads Between Sub Lands
     * @param subLands
     * @return
     */
    vector<Polygon1> buildRoads(vector<Polygon1> &subLands);

    /**
     * Set Home inside all SubLands
     * @param home
     * @return
     */
    vector<Polygon1> setHomesInsideSubLands(const Polygon1 &home , int greenAreaLand = 1);

    /**
     * Set Home Inside Land
     * @param home
     * @return
     */
    Polygon1 setHomesInsideLand(const Polygon1 &home , const Polygon1 &curLand , double preferDegree = 0 );

    map<int , map<Point , bool>> storePointsShared(const vector<Polygon1> &polys);
};


#endif //DESIGNI_LAND_H
