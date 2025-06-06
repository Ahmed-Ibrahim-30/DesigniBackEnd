//
// Created by ahmed Ibrahim on 22-Feb-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONROADSBYDIVISIONSCOUNT_H
#define DESIGNI_BACKEND_LANDDIVISIONROADSBYDIVISIONSCOUNT_H
#include "LandDivisionRoads.h"

class LandDivisionRoadsByDivisionsCount :public LandDivisionRoads{
private:
    Polygon1 mainLand;
    void divideLand(double ratioA, double ratioB, int maxDivisions, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans, LandDivisionSortingStrategy  landDivisionStrategy);
public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double ratioA, double ratioB, int maxDivisions,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    LandDivisionRoadsByDivisionsCount(double divisionArea, double externalRoad, double centralRoad,
                                      double circularStreet, double landDepth, double streetCut);

    vector<vector<Polygon1>>
    divideLands(vector<Polygon1> &lands, double ratioA, double ratioB, int maxDivisions,LandDivisionSortingStrategy  landDivisionStrategy) override;
};


#endif //DESIGNI_BACKEND_LANDDIVISIONROADSBYDIVISIONSCOUNT_H
