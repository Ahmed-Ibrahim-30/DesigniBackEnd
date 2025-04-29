//
// Created by ahmed Ibrahim on 29-Apr-25.
//

#ifndef DESIGNI_BACKEND_ADJGREENAREA_H
#define DESIGNI_BACKEND_ADJGREENAREA_H
#include "GreenAreaSelector.h"

/**
 * Select Green Areas based on most connected Lands with each Adj
 */
class AdjGreenArea : public GreenAreaSelector{
public:
    void select( Polygon1 &outerLand ,vector<Polygon1> &lands , double greenAreaPercentage , double totalGreenAreas ) override;
};


#endif //DESIGNI_BACKEND_ADJGREENAREA_H
