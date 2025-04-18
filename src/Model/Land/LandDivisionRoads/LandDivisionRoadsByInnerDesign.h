//
// Created by ahmed Ibrahim on 23-Feb-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONROADSBYINNERDESIGN_H
#define DESIGNI_BACKEND_LANDDIVISIONROADSBYINNERDESIGN_H
#include "LandDivisionRoads.h"

class LandDivisionRoadsByInnerDesign :public LandDivisionRoads{
private:
    int sol = 0;
    Polygon1 mainLand;

    void divideLand(const Polygon1 &innerHome , vector<Polygon1> &pols , vector<vector<Polygon1>> &ans ,LandDivisionSortingStrategy  landDivisionStrategy);

public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land, const Polygon1 &innerHome,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    LandDivisionRoadsByInnerDesign(double divisionArea, double externalRoad, double centralRoad, double circularStreet,
                                   double landDepth, double streetCut);

    vector<vector<Polygon1>> divideLands(vector<Polygon1> &lands, const Polygon1 &innerHome,LandDivisionSortingStrategy  landDivisionStrategy ) override;

};


#endif //DESIGNI_BACKEND_LANDDIVISIONROADSBYINNERDESIGN_H
