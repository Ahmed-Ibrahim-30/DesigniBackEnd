//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#include "SortLandDivisionsByMaximizedConnections.h"
#include "src/Model/PolygonLibrary/PolygonAdjacencyAnalyzer.h"

vector<vector<Polygon1>> SortLandDivisionsByMaximizedConnections::sortDivisions(vector<vector<Polygon1>> &divisions)
{
    if(divisions.size() <= 1)
    {
        return divisions;
    }

    vector<vector<Polygon1>> sortedLands;

    vector<pair<int , int >> sortSolutions;

    for (int i = 0; i < divisions.size(); ++i)
    {
        vector<vector<int>> landsAdj = PolygonAdjacencyAnalyzer::getAdj(divisions[i]);

        int adjCount = 0;

        for(auto &land : landsAdj)adjCount += (int) land.size();
        adjCount /= 2;
        sortSolutions.emplace_back(adjCount , i);
    }

    sort(sortSolutions.begin(), sortSolutions.end() , greater<>());

    for(auto &sorted : sortSolutions)
    {
        sortedLands.push_back(divisions[sorted.second]);
    }

    return sortedLands;
}
