//
// Created by ahmed Ibrahim on 15-Feb-25.
//

#ifndef DESIGNI_BACKEND_SORTLANDDIVISIONS_H
#define DESIGNI_BACKEND_SORTLANDDIVISIONS_H
#include "src/Model/Polygon.h"
#include "src/Model/Line.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"

enum LandDivisionSortingStrategy{
    MinimizingLengthVariance = 0,
    MinimizeSmallDimensions,
    MinimizeAcuteAngles,
    MinimizeLandConnections,
    MaximizeLandConnections
};

class SortLandDivisions {
public:

    /**
     * sort Divisions
     * @param polygons
     * @return
     */
    virtual vector<vector<Polygon1>> sortDivisions(vector<vector<Polygon1>> &divisions) = 0;

    /**
     * Sort Divisions By Two Ratios
     * @param divisions
     * @return
     */
    virtual vector<vector<Polygon1>> sortDivisions(vector<vector<Polygon1>> &divisions , double ratio1 , double ratio2)
    {
        vector<pair<double , int>> solutions;

        for (int i = 0; i < divisions.size(); ++i)
        {
            int n = (int)divisions[i].size();;
            Polygon1 pol1 = divisions[i][n-2];
            Polygon1 pol2 = divisions[i][n-1];

            double area1 = pol1.getArea();
            double area2 = pol2.getArea();
            double factor = abs(max(area1 , area2) / min(area1 , area2) - max(ratio1 , ratio2) / min(ratio1 , ratio2));

            solutions.emplace_back(factor , i);
        }
        std::sort(solutions.begin(), solutions.end());
        vector<vector<Polygon1>> ans;
        for(auto &sol : solutions)
        {
            ans.push_back(divisions[sol.second]);
        }
        return ans;
    };
};


#endif //DESIGNI_BACKEND_SORTLANDDIVISIONS_H
