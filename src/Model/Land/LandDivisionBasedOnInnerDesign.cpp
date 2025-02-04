//
// Created by ahmed Ibrahim on 15-Jan-25.
//

#include "LandDivisionBasedOnInnerDesign.h"
#include "src/Model/DesignGeometryManager.h"

vector<vector<Polygon1>> LandDivisionBasedOnInnerDesign::divideLand(const Polygon1 &land, const Polygon1 &innerHome) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( innerHome , pols , ans);

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
LandDivisionBasedOnInnerDesign::divideLands(vector<Polygon1> &lands, const Polygon1 &innerHome) {
    vector<vector<Polygon1>> ans;
    divideLand( innerHome , lands , ans);

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


void LandDivisionBasedOnInnerDesign::divideLand(const Polygon1 &innerHome1 , vector<Polygon1> &pols , vector<vector<Polygon1>> &ans) {


    if(sol >=80) { ans.push_back(pols); return; }
    cout<<"Solution Index = "<<sol++ <<"\n";
    Polygon1 innerHome = innerHome1;
    bool foundedSol = false ; int polygonIndex = -1;

    for (int i = 0; i < pols.size(); ++i)
    {

        if(!pols[i].isDivisible()) continue;//if land is green Area

        polygonIndex = i;
        auto polygonDivided = pols[polygonIndex];
        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

        if(paiPoly.empty()) continue;

        int index = 0;
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
            paiPoly2.push_back(div);
        }

        if (paiPoly2.empty()) continue;

        foundedSol = true;

        vector<Polygon1> newPolygon ;
        for (int j = 0; j < pols.size(); ++j)
        {
            if (j == polygonIndex) continue;
            newPolygon.push_back(pols[j]);
        }

        int divIndex = 0;
        auto div = paiPoly2[divIndex];

        if (pols.size() == 1)
        {
            for(auto &sol : paiPoly2)
            {
                newPolygon.push_back(sol.first);
                newPolygon.push_back(sol.second);

                divideLand( innerHome , newPolygon , ans);

                newPolygon.pop_back();
                newPolygon.pop_back();
            }
        }
        else
        {
            newPolygon.push_back(div.first);
            newPolygon.push_back(div.second);

            divideLand( innerHome , newPolygon , ans);

            newPolygon.pop_back();
            newPolygon.pop_back();
        }

        if (foundedSol) break;
    }

    if(! foundedSol)
    {
        ans.push_back(pols);
        return;
    }
}



