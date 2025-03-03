//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#ifndef DESIGNI_BACKEND_CENTROIDLINEGREENSELECTOR_H
#define DESIGNI_BACKEND_CENTROIDLINEGREENSELECTOR_H
#include "GreenAreaSelector.h"

class CentroidLineGreenSelector : public GreenAreaSelector{
public:
    void select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas) override;
};


#endif //DESIGNI_BACKEND_CENTROIDLINEGREENSELECTOR_H
