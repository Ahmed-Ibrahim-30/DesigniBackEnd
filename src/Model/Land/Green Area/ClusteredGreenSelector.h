//
// Created by ahmed Ibrahim on 30-Jan-25.
//

#ifndef DESIGNI_CLUSTEREDGREENSELECTOR_H
#define DESIGNI_CLUSTEREDGREENSELECTOR_H
#include "GreenAreaSelector.h"

class ClusteredGreenSelector : public GreenAreaSelector{
public:
    void select(Polygon1 &outerLand ,vector<Polygon1> &lands , double greenAreaPercentage) override;
};


#endif //DESIGNI_CLUSTEREDGREENSELECTOR_H
