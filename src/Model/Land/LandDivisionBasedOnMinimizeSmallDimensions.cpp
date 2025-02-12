//
// Created by ahmed Ibrahim on 12-Feb-25.
//

#include "LandDivisionBasedOnMinimizeSmallDimensions.h"

void LandDivisionBasedOnMinimizeSmallDimensions::divideLand(double ratioA, double ratioB, int maxDivisions,vector<Polygon1> &pols, vector<vector<Polygon1>> &ans)
{
    if(pols.size() >= maxDivisions)
    {
        ans.push_back(pols);
        return;
    }

    int polygonIndex = -1;

    vector<tuple<double , int , int >> sortSolutions;
    vector<vector<pair<Polygon1 , Polygon1>>> solutions(pols.size());
    for (int i = 0; i < pols.size(); ++i)
    {
        if(!pols[i].isDivisible()) continue;//if land is green Area

        vector<Polygon1> newPolygons;
        for (int var = 0; var < pols.size(); ++var) {
            if(var == i) continue;
            newPolygons.push_back(pols[var]);
        }

        auto polygonDivided = pols[i];

        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

        for (int j = 0; j < paiPoly.size(); ++j) {
            auto div = paiPoly[j];

            newPolygons.push_back(div.first);
            newPolygons.push_back(div.second);
            vector<Polygon1> pps = {div.first , div.second};

            double minLength = 10000000000;
            for(auto &p : pps)
            {
                minLength = min(minLength , p.getMINSideLength());
            }

            sortSolutions.emplace_back(minLength , i , j);
            solutions[i].push_back(paiPoly[j]);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }
    }

    int  second;  double t;
    if(sortSolutions.empty())return;

    sort(sortSolutions.begin() , sortSolutions.end() , greater<>());

    tie(t , polygonIndex , second) = sortSolutions[0];


    vector<Polygon1> newPolygon ;
    for (int j = 0; j < pols.size(); ++j)
    {
        if (j == polygonIndex) continue;
        newPolygon.push_back(pols[j]);
    }

    auto div = solutions[polygonIndex][second];


    newPolygon.push_back(div.first);
    newPolygon.push_back(div.second);

    divideLand( ratioA , ratioB , maxDivisions , newPolygon , ans);

    newPolygon.pop_back();
    newPolygon.pop_back();

}

vector<vector<Polygon1>>LandDivisionBasedOnMinimizeSmallDimensions::divideLand(const Polygon1 &land, double ratioA, double ratioB,int maxDivisions)
{
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , pols , ans);

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
LandDivisionBasedOnMinimizeSmallDimensions::divideLands(vector<Polygon1> &lands, double ratioA, double ratioB,int maxDivisions)
{
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , lands , ans);

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
