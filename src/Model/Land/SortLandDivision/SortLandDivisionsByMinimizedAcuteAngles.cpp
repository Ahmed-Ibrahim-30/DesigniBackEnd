//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#include "SortLandDivisionsByMinimizedAcuteAngles.h"

vector<vector<Polygon1>> SortLandDivisionsByMinimizedAcuteAngles::sortDivisions(vector<vector<Polygon1>> &divisions)
{

    if(divisions.size() <= 1)
    {
        return divisions;
    }

    vector<vector<Polygon1>> sortedLands;

    vector<pair<int , int >> sortSolutions;

    for (int i = 0; i < divisions.size(); ++i)
    {
        int n = (int)divisions[i].size();
        int acuteLinesCount = divisions[i][n-2].countAcuteAnglesLines() ;
        acuteLinesCount += divisions[i][n-1].getMINSideLength();
        sortSolutions.emplace_back(acuteLinesCount , i);
    }

    sort(sortSolutions.begin(), sortSolutions.end() );

    for(auto &sorted : sortSolutions)
    {
        sortedLands.push_back(divisions[sorted.second]);
    }

    return sortedLands;
}
