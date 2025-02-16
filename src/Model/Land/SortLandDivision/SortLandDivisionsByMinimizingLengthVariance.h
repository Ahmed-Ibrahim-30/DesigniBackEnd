//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#ifndef DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZINGLENGTHVARIANCE_H
#define DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZINGLENGTHVARIANCE_H
#include "SortLandDivisions.h"

/**
 * Sort Based On Lands Lines Close Together
 * This is Default Strategy
 */
class SortLandDivisionsByMinimizingLengthVariance :public SortLandDivisions{
public:
    vector<vector<Polygon1>> sortDivisions(vector<vector<Polygon1>> &divisions) override;
};


#endif //DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZINGLENGTHVARIANCE_H
