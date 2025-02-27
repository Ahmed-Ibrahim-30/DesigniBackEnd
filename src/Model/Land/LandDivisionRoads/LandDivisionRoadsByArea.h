//
// Created by ahmed Ibrahim on 23-Feb-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONROADSBYAREA_H
#define DESIGNI_BACKEND_LANDDIVISIONROADSBYAREA_H
#include "LandDivisionRoads.h"

class LandDivisionRoadsByArea :public LandDivisionRoads{
private:
    int sol = 0;

    Polygon1 mainLand;

    void divideLand(double area, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans , LandDivisionSortingStrategy  landDivisionStrategy);

public:
    vector<vector<Polygon1>> divideLands(vector<Polygon1> &lands, double area,LandDivisionSortingStrategy  landDivisionStrategy ) override;

    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double area,LandDivisionSortingStrategy  landDivisionStrategy) override;

};


#endif //DESIGNI_BACKEND_LANDDIVISIONROADSBYAREA_H
