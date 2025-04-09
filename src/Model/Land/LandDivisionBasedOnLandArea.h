//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#ifndef DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
#define DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
#include "LandDivision.h"

class LandDivisionBasedOnLandArea : public LandDivision{
private:
    int sol = 0;

    void divideLand(double area, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans , LandDivisionSortingStrategy  landDivisionStrategy);

public:
    vector<vector<Polygon1>> divideLands(vector<Polygon1> &lands, double area,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    LandDivisionBasedOnLandArea(double divisionArea, double externalRoad, double centralRoad, double circularStreet,
                                double landDepth, double streetCut);

    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double area,LandDivisionSortingStrategy  landDivisionStrategy) override;

};


#endif //DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
