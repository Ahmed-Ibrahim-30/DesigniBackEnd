//
// Created by ahmed Ibrahim on 15-Jan-25.
//

#include "LandDivisionBasedOnInnerDesign.h"
#include "src/Model/DesignGeometryManager.h"

vector<vector<Polygon1>> LandDivisionBasedOnInnerDesign::divideLand(const Polygon1 &land, const Polygon1 &innerHome, LandDivisionSortingStrategy  landDivisionStrategy)
{
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( innerHome , pols , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    for(auto &pol : ans)
    {
        cout<<"********************************\n";
        for (int i = 0; i < pol.size(); ++i)
        {
            cout<<"*************** "<<i<<" *****************\n";
            pol[i].print();
            // cout<<"********************************\n";
        }
        break;
    }

    return ans;
}

vector<vector<Polygon1>>
LandDivisionBasedOnInnerDesign::divideLands(vector<Polygon1> &lands, const Polygon1 &innerHome, LandDivisionSortingStrategy  landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( innerHome , lands , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    if(!ans.empty())
    {
//        for(auto &pol : ans)
//        {
//            for (int i = 0; i < pol.size(); ++i)
//            {
//                cout<<"*************** "<<i<<" *****************\n";
//                pol[i].print();
//                // cout<<"********************************\n";
//            }
//        }
    }

    return ans;
}


void LandDivisionBasedOnInnerDesign::divideLand(const Polygon1 &innerHome1 , vector<Polygon1> &pols , vector<vector<Polygon1>> &ans ,LandDivisionSortingStrategy  landDivisionStrategy)
{

    if(sol >= 80) { ans.push_back(pols); return; }

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
            break;
        case MaximizeLandConnections:
            break;
    }

    cout<<"Division Index = "<<sol++ <<"\n";
    Polygon1 innerHome = innerHome1;
    bool foundedSol = false ;

    pols = PolygonHelper::sortPolygonByArea(pols);

    vector<vector<Polygon1>> possibleDivisions;

    for (int i = 0; i < pols.size(); ++i)
    {

        if(!pols[i].isDivisible()) continue;//if land is green Area

        auto polygonDivided = pols[i];
        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

        if(paiPoly.empty()) continue;

        vector<Polygon1> newPolygons ;
        for (int j = 0; j < pols.size(); ++j)
        {
            if (j == i) continue;
            newPolygons.push_back(pols[j]);
        }

        vector<pair<Polygon1 , Polygon1>> paiPoly2;

        for(auto &div : paiPoly)
        {
            innerHome = innerHome1;
            bool validPol1 = DesignGeometryManager::positionPolygonInsideAnotherUsingAllAngles(div.first , innerHome);
            innerHome = innerHome1;
            bool validPol2 = DesignGeometryManager::positionPolygonInsideAnotherUsingAllAngles(div.second , innerHome);


            if(!validPol1 || !validPol2)
            {
                continue;
            }

            newPolygons.push_back(div.first);
            newPolygons.push_back(div.second);

            possibleDivisions.push_back(newPolygons);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }

        if (possibleDivisions.empty()) continue;

        possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions);

        foundedSol = true;

        if (pols.size() == 1)
        {
            for(auto &solution : possibleDivisions)
            {
                divideLand( innerHome , solution , ans , landDivisionStrategy);
            }
        }
        else
        {
            divideLand( innerHome , possibleDivisions[0] , ans , landDivisionStrategy);
        }
    }

    if(! foundedSol)
    {
        ans.push_back(pols);
        return;
    }
}



