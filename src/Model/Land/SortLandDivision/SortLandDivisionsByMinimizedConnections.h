//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#ifndef DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZEDCONNECTIONS_H
#define DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZEDCONNECTIONS_H
#include "SortLandDivisions.h"

class SortLandDivisionsByMinimizedConnections :public SortLandDivisions{
public:
    vector<vector<Polygon1>> sortDivisions(vector<vector<Polygon1>> &divisions) override;

};


#endif //DESIGNI_BACKEND_SORTLANDDIVISIONSBYMINIMIZEDCONNECTIONS_H
