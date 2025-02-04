//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#ifndef DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
#define DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
#include "LandDivision.h"

class LandDivisionBasedOnSidesConvergence :public LandDivision{
private:
    void divideLand(double ratioA, double ratioB, int maxDivisions, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans);

    vector<double> getPolygonsDiffLength(const vector<Polygon1> &pol);
public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double ratioA, double ratioB, int maxDivisions) override;

    vector<vector<Polygon1>>
    divideLands(vector<Polygon1> &lands, double ratioA, double ratioB, int maxDivisions) override;
};


#endif //DESIGNI_LANDDIVISIONBASEDONSIDESCONVERGENCE_H
