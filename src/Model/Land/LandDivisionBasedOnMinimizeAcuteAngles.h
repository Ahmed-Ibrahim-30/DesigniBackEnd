//
// Created by ahmed Ibrahim on 12-Feb-25.
//

#ifndef DESIGNI_BACKEND_LANDDIVISIONBASEDONMINIMIZEACUTEANGLES_H
#define DESIGNI_BACKEND_LANDDIVISIONBASEDONMINIMIZEACUTEANGLES_H
#include "LandDivision.h"

class LandDivisionBasedOnMinimizeAcuteAngles :public LandDivision{
private:
    void divideLand(double ratioA, double ratioB, int maxDivisions, vector<Polygon1> &pols , vector<vector<Polygon1>> &ans);

public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land ,double ratioA, double ratioB, int maxDivisions) override;

    vector<vector<Polygon1>>
    divideLands(vector<Polygon1> &lands, double ratioA, double ratioB, int maxDivisions) override;
};


#endif //DESIGNI_BACKEND_LANDDIVISIONBASEDONMINIMIZEACUTEANGLES_H
