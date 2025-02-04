//
// Created by ahmed Ibrahim on 15-Jan-25.
//

#ifndef DESIGNI_LANDDIVISIONBASEDONINNERDESIGN_H
#define DESIGNI_LANDDIVISIONBASEDONINNERDESIGN_H
#include "LandDivision.h"

class LandDivisionBasedOnInnerDesign : public LandDivision{
private:
    int sol = 0;

    void divideLand(const Polygon1 &innerHome , vector<Polygon1> &pols , vector<vector<Polygon1>> &ans);

public:
    vector<vector<Polygon1>> divideLand(const Polygon1 &land, const Polygon1 &innerHome) override;

    vector<vector<Polygon1>> divideLands(vector<Polygon1> &lands, const Polygon1 &innerHome) override;

};


#endif //DESIGNI_LANDDIVISIONBASEDONINNERDESIGN_H
