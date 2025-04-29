//
// Created by ahmed Ibrahim on 22-Feb-25.
//

#include "LandDivisionRoadsByDivisionsCount.h"

vector<vector<Polygon1>>LandDivisionRoadsByDivisionsCount::divideLand(const Polygon1 &land, double ratioA, double ratioB, int maxDivisions,LandDivisionSortingStrategy landDivisionStrategy)
{
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;

    mainLand = land;

    divideLand( ratioA , ratioB , maxDivisions , pols , ans , landDivisionStrategy);

    return ans;
}

vector<vector<Polygon1>>
LandDivisionRoadsByDivisionsCount::divideLands(vector<Polygon1> &lands, double ratioA, double ratioB, int maxDivisions,
                                               LandDivisionSortingStrategy landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , lands , ans , landDivisionStrategy);


    return ans;
}

void LandDivisionRoadsByDivisionsCount::divideLand(double ratioA, double ratioB, int maxDivisions, vector<Polygon1> &pols,
                                              vector<vector<Polygon1>> &ans,
                                              LandDivisionSortingStrategy landDivisionStrategy){
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

    set<string> curIds;
    for(auto &str : pols)
    {
        curIds.insert(str.getId());
    }

    vector<vector<Polygon1>> possibleDivisions;
    vector<string> newTwoIds {to_string(pols.size()+1), to_string(pols.size()+2)};
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

        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided , sqrt(mainLand.getArea() / 100000));

        for (const auto& div : paiPoly)
        {
            newPolygons.push_back(div.first);
            newPolygons.back().setId(newTwoIds[0]);
            newPolygons.push_back(div.second);
            newPolygons.back().setId(newTwoIds[1]);

            possibleDivisions.emplace_back(newPolygons);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }
        if(!paiPoly.empty())break;
    }

    if(possibleDivisions.empty())return;

    possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions);

    divideLand( ratioA , ratioB , maxDivisions , possibleDivisions[0] , ans , landDivisionStrategy);

}

LandDivisionRoadsByDivisionsCount::LandDivisionRoadsByDivisionsCount(double divisionArea, double externalRoad,
                                                                     double centralRoad, double circularStreet,
                                                                     double landDepth, double streetCut)
        : LandDivisionRoads(divisionArea, externalRoad, centralRoad, circularStreet, landDepth, streetCut) {}




