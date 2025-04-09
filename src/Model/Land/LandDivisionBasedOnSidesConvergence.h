//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#ifndef DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
#define DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
#include "LandDivision.h"

class LandDivisionBasedOnSidesConvergence :public LandDivision{
private:
    void divideLand(double ratioA, double ratioB, int maxDivisions, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans, LandDivisionSortingStrategy  landDivisionStrategy);

public:
    LandDivisionBasedOnSidesConvergence(double divisionArea, double externalRoad, double centralRoad,
                                        double circularStreet, double landDepth, double streetCut);

    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double ratioA, double ratioB, int maxDivisions,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    vector<vector<Polygon1>>
    divideLands(vector<Polygon1> &lands, double ratioA, double ratioB, int maxDivisions,LandDivisionSortingStrategy  landDivisionStrategy) override;
};


#endif //DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
