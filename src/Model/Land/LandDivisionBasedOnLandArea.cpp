//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#include "LandDivisionBasedOnLandArea.h"

vector<vector<Polygon1>> LandDivisionBasedOnLandArea::divideLand(const Polygon1 &land ,double area , LandDivisionSortingStrategy  landDivisionStrategy)
{
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( area , pols , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    return ans;
}

vector<vector<Polygon1>> LandDivisionBasedOnLandArea::divideLands(vector<Polygon1> &lands, double area , LandDivisionSortingStrategy  landDivisionStrategy)
{
    vector<vector<Polygon1>> ans;
    divideLand( area , lands , ans , landDivisionStrategy);

    sort(ans.begin(), ans.end() , [](vector<Polygon1> &pol1 , vector<Polygon1> &pol2){
        return pol1.size() > pol2.size();
    });

    cout<<"Ans Size = "<<ans.size()<<"\n";

    if(!ans.empty())
    {
//        for(auto &pol : ans)
//        {
//            cout<<"********************************\n";
//            for(auto &t : pol) t.print();
//        }
    }

    return ans;
}

void LandDivisionBasedOnLandArea::divideLand(double area , vector<Polygon1> &pols, vector<vector<Polygon1>> &ans , LandDivisionSortingStrategy  landDivisionStrategy)
{
    if(pols.size() > 200)
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

        for (auto div : paiPoly)
        {
            double area1 = div.first.getArea();
            double area2 = div.second.getArea();
            if(area1 < area || area2 < area)
            {
                continue;
            }

            newPolygons.push_back(div.first);
            newPolygons.push_back(div.second);

            possibleDivisions.push_back(newPolygons);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }
        if(!paiPoly.empty())break;
    }

    if(possibleDivisions.empty())
    {
        sol = -1;
        ans.push_back(pols);
        return;
    }

    possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions);

    if (pols.size() == 1)
    {
        for(auto &solution : possibleDivisions)
        {
            divideLand( area , solution , ans , landDivisionStrategy);
        }
    }
    else
    {
        divideLand( area , possibleDivisions[0] , ans , landDivisionStrategy);
    }
}

LandDivisionBasedOnLandArea::LandDivisionBasedOnLandArea(double divisionArea, double externalRoad, double centralRoad,
                                                         double circularStreet, double landDepth, double streetCut)
        : LandDivision(divisionArea, externalRoad, centralRoad, circularStreet, landDepth, streetCut) {}



