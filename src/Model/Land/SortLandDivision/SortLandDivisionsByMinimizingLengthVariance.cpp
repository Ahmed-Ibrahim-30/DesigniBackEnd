//
// Created by ahmed Ibrahim on 16-Feb-25.
//

#include "SortLandDivisionsByMinimizingLengthVariance.h"

vector<vector<Polygon1>>SortLandDivisionsByMinimizingLengthVariance::sortDivisions(vector<vector<Polygon1>> &divisions)
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

        vector<Line> lines;
        for (int j = n-1; j >= n-2; j--)
        {
            for(auto &line : divisions[i][j].getLines())
            {
                lines.push_back(line);
            }
        }

        vector<double> lineLengths;

        for(auto &line : lines)
        {
            double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();
            double lengthV = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            lineLengths.push_back(lengthV);
        }
        std::sort(lineLengths.begin(), lineLengths.end());

        double lengthVariance = lineLengths.back() - lineLengths[0];
        sortSolutions.emplace_back(lengthVariance , i);
    }

    sort(sortSolutions.begin(), sortSolutions.end() );

    for(auto &sorted : sortSolutions)
    {
        sortedLands.push_back(divisions[sorted.second]);
    }

    return sortedLands;
}
