//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#include "LandDivisionBasedOnSidesConvergence.h"
vector<vector<Polygon1>>
LandDivisionBasedOnSidesConvergence::divideLand(const Polygon1 &land, double ratioA, double ratioB, int maxDivisions, LandDivisionSortingStrategy  landDivisionStrategy) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , pols , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    int index = 0;
    if(!ans.empty())
    {
        for(auto &pol : ans[0])
        {
            index++;
            // if(index != 5) continue;
            pol.print();
        }
    }

    return ans;
}

vector<vector<Polygon1>>
LandDivisionBasedOnSidesConvergence::divideLands(vector<Polygon1> &lands, double ratioA , double ratioB , int maxDivisions, LandDivisionSortingStrategy  landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , lands , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    int index = 0;
    if(!ans.empty())
    {
//        for(auto &pol : ans[0])
//        {
//            index++;
//            // if(index != 5) continue;
//            pol.print();
//        }
    }

    return ans;
}

void
LandDivisionBasedOnSidesConvergence::divideLand(double ratioA, double ratioB, int maxDivisions,
                                                vector<Polygon1> &pols,
                                                vector<vector<Polygon1>> &ans , LandDivisionSortingStrategy  landDivisionStrategy) {
    if(pols.size() >= maxDivisions)
    {
        ans.push_back(pols);
        return;
    }

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

    pols = PolygonHelper::sortPolygonByArea(pols);

    vector<vector<Polygon1>> possibleDivisions;
    for (int i = 0; i < pols.size(); ++i)
    {
        if(!pols[i].isDivisible()) continue;//if land is green Area

        vector<Polygon1> newPolygons;
        for (int var = 0; var < pols.size(); ++var)
        {
            if(var == i) continue;
            newPolygons.push_back(pols[var]);
        }

        auto polygonDivided = pols[i];

        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

        for (const auto& div : paiPoly)
        {
            newPolygons.push_back(div.first);
            newPolygons.push_back(div.second);

            possibleDivisions.push_back(newPolygons);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }
        if(!paiPoly.empty())break;
    }

    if(possibleDivisions.empty())return;

    possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions);

    divideLand( ratioA , ratioB , maxDivisions , possibleDivisions[0] , ans , landDivisionStrategy);

}

