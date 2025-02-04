//
// Created by ahmed Ibrahim on 30-Jan-25.
//

#ifndef DESIGNI_UNIFORMGREENDISTRIBUTOR_H
#define DESIGNI_UNIFORMGREENDISTRIBUTOR_H
#include "GreenAreaSelector.h"

/**
 * distribute green area between all lands as Possible
 * So that each green area between the largest number of lands as possible
 */
class UniformGreenDistributor : public GreenAreaSelector{
public:
    void select( Polygon1 &outerLand ,vector<Polygon1> &lands , double greenAreaPercentage) override;
};


#endif //DESIGNI_UNIFORMGREENDISTRIBUTOR_H
