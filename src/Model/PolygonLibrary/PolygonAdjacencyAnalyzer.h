//
// Created by ahmed Ibrahim on 29-Jan-25.
//

#ifndef DESIGNI_POLYGONADJACENCYANALYZER_H
#define DESIGNI_POLYGONADJACENCYANALYZER_H
#include "src/Model/Polygon.h"

class PolygonAdjacencyAnalyzer {
public:
    /**
     * Get Adjacency between Polygons By Index
     * 0 -- > 3 4 5 and so on
     * @param polygons
     * @return
     */
    static vector<vector<int>> getAdj(vector<Polygon1> &polygons);
};


#endif //DESIGNI_POLYGONADJACENCYANALYZER_H
