//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#ifndef DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
#define DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
#include "LandDivision.h"

class LandDivisionBasedOnLandArea : public LandDivision{
private:

    int sol = 0;

    void divideLand(double area, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans);

public:
    vector<vector<Polygon1>> divideLands(vector<Polygon1> &lands, double area) override;

public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double area) override;

};


#endif //DESIGNI_LANDDIVISIONBASEDONLANDAREA_H
