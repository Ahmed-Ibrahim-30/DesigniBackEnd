//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#include "SortLandDivisionsByMinimizedDimensions.h"

vector<vector<Polygon1>> SortLandDivisionsByMinimizedDimensions::sortDivisions(vector<vector<Polygon1>> &divisions)
{

    if(divisions.size() <= 1)
    {
        return divisions;
    }

    vector<vector<Polygon1>> sortedLands;

    vector<pair<double , int >> sortSolutions;

    for (int i = 0; i < divisions.size(); ++i)
    {
        int n = (int)divisions[i].size();
        double minLength = min(divisions[i][n-2].getMINSideLength() , divisions[i][n-1].getMINSideLength());
        sortSolutions.emplace_back(minLength , i);
    }

    sort(sortSolutions.begin(), sortSolutions.end() , greater<>());

    for(auto &sorted : sortSolutions)
    {
        sortedLands.push_back(divisions[sorted.second]);
    }

    return sortedLands;
}
