//
// Created by ahmed Ibrahim on 29-Apr-25.
//

#include "AdjGreenArea.h"

void
AdjGreenArea::select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas)
{
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
    cout<<"GREEN AREA = "<<greenAreas<<"\n";

    vector<vector<int>> adj = PolygonAdjacencyAnalyzer::getAdjByCentroids(lands);

    vector<tuple<int , double , int>> sortPolygons;
    for (int i = 0; i < lands.size(); ++i)
    {
        sortPolygons.emplace_back(adj[i].size() , lands[i].getArea() , i);
    }
    sort(sortPolygons.begin(), sortPolygons.end() , greater<>());

    for (int i = 0; i < greenAreas; ++i)
    {
        int adjSize , index; double area;
        tie(adjSize , area , index) = sortPolygons[i];
        lands[index].setDivisible(false);
    }
}
