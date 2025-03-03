//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#include "LandDivisionBasedOnRatios.h"

vector<vector<Polygon1>> LandDivisionBasedOnRatios::divideLand(const Polygon1 &land, const vector<double> &ratios,
                                                               LandDivisionSortingStrategy landDivisionStrategy) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;

    mainLand = land;

    divideLand( ratios, pols , ans , landDivisionStrategy);

    return ans;
}

vector<vector<Polygon1>> LandDivisionBasedOnRatios::divideLands(vector<Polygon1> &lands, const vector<double> &ratios,
                                                                LandDivisionSortingStrategy landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( ratios , lands , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    if(!ans.empty())
    {
        for(auto &pol : ans[0])
        {
            pol.print();
        }
    }

    return ans;
}

void LandDivisionBasedOnRatios::divideLand(const vector<double> &ratios, vector<Polygon1> &pols,vector<vector<Polygon1>> &ans,
                                           LandDivisionSortingStrategy landDivisionStrategy){

    int divisions = ratios.size();

    if(pols.size() >= divisions)
    {
        ans.push_back(pols);
        return;
    }

    int curIndex = pols.size();

    SortLandDivisions *sortLandDivisions ;
    switch (landDivisionStrategy)
    {
        case MinimizingLengthVariance:
            sortLandDivisions = new SortLandDivisionsByMinimizingLengthVariance();
            break;
        case MinimizeSmallDimensions:
            sortLandDivisions = new SortLandDivisionsByMinimizedDimensions();
            break;
        case MinimizeAcuteAngles:
            sortLandDivisions = new SortLandDivisionsByMinimizedAcuteAngles();
            break;
        case MinimizeLandConnections:
            sortLandDivisions = new SortLandDivisionsByMinimizedConnections();
            break;
        case MaximizeLandConnections:
            sortLandDivisions = new SortLandDivisionsByMaximizedConnections();
            break;
    }


    vector<vector<Polygon1>> possibleDivisions;

    vector<Polygon1> newPolygons = pols;
    newPolygons.pop_back();

    auto polygonDivided = pols.back();

    vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

    for (const auto& div : paiPoly)
    {
        newPolygons.push_back(div.first);
        newPolygons.push_back(div.second);

        possibleDivisions.push_back(newPolygons);

        newPolygons.pop_back();
        newPolygons.pop_back();
    }

    if(possibleDivisions.empty())
    {
        ans.push_back(pols);
        return;
    }
    double sumOtherRatio = 0;
    for (int i = curIndex+1; i < ratios.size(); ++i) {
        sumOtherRatio += ratios[i];
    }

    possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions , ratios[curIndex] , sumOtherRatio);

    vector<Polygon1> selectedSolution = possibleDivisions[0];
    int n = selectedSolution.size();

    if (selectedSolution[n-1].getArea() < selectedSolution[n-2].getArea())
    {
        swap(selectedSolution[n-1] , selectedSolution[n-2]);
    }
    divideLand( ratios , selectedSolution , ans , landDivisionStrategy);
}
