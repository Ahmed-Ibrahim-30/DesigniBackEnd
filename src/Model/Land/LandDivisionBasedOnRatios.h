//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONBASEDONRATIOS_H
#define DESIGNI_BACKEND_LANDDIVISIONBASEDONRATIOS_H
#include "LandDivision.h"

class LandDivisionBasedOnRatios :public LandDivision{
private:
    Polygon1 mainLand;
    void divideLand(const vector<double> &ratios, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans, LandDivisionSortingStrategy  landDivisionStrategy);

public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,const vector<double> &ratios,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    vector<vector<Polygon1>>divideLands(vector<Polygon1> &lands,const vector<double> &ratios,LandDivisionSortingStrategy  landDivisionStrategy) override;
};


#endif //DESIGNI_BACKEND_LANDDIVISIONBASEDONRATIOS_H
