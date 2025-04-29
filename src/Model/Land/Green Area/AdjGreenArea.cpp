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

    for (int i = 0; i < greenAreas; ++i)
    {
        sort(sortPolygons.begin(), sortPolygons.end() , greater<>());
        int adjSize , index; double area;
        tie(adjSize , area , index) = sortPolygons[0];
        lands[index].setDivisible(false);
        set<int> arr;

        for(auto &ne : adj[index])
        {
            arr.insert(ne);
        }
        for (auto & sortPolygon : sortPolygons) {
            int adjSize2 , index2; double area2;
            tie(adjSize2 , area2 , index2) = sortPolygon;
            if (arr.count(index))get<0>(sortPolygon) --;
            else if (index2 == index)get<0>(sortPolygon) =0;
        }
    }
}
