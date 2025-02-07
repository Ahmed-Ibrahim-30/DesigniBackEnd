//
// Created by ahmed Ibrahim on 30-Jan-25.
//

#include "ClusteredGreenSelector.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "queue"
#include "src/Model/DesignGeometryManager.h"

void ClusteredGreenSelector::select(Polygon1 &outerLand ,vector<Polygon1> &lands, double greenAreaPercentage) {
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
//    cout<<"GREEN AREA = "<<greenAreas<<"\n";

    Point outerCentroid = outerLand.calculateCentroid();
    vector<Line> outerLine = outerLand.getLines();

    vector<vector<int>> landsAdj = PolygonAdjacencyAnalyzer::getAdj(lands);

    vector<set<int>> landsAdjSet(n);

    for (int i = 0; i < landsAdj.size(); ++i)
    {
        for(auto &adj : landsAdj[i])landsAdjSet[i].insert(adj);
    }

    vector<pair<double , int>> diffCentroid;
    for (int i = 0; i < lands.size(); ++i)
    {
        Point centerLand = lands[i].calculateCentroid();

        double x1 = outerCentroid.getX() , y1 = outerCentroid.getY();
        double x2 = centerLand.getX() , y2 = centerLand.getY();

        double diff = sqrt(((x2 - x1) * (x2 - x1)) + (y2 - y1) * (y2 - y1));

        bool inBoundry = false;
        double area = lands[i].getArea();

        for (int j = 0; j < lands[i].getPoints().size(); ++j)
        {
            Point a1 = lands[i].getPoints()[j];
            Point a2 = lands[i].getPoints()[(j+1) % lands[i].getPoints().size()];

            for(auto &line : outerLine)
            {
                Point l1 (line.getX1() , line.getY1());
                Point l2 (line.getX2() , line.getY2());

//                if(DesignGeometryManager::doSegmentsIntersect(a1 , a2 , l1 , l2)||
//                        a1 == l1 || a1 == l2 || a2 == l1 || a2 == l2)
//                {
//                    inBoundry = true;
//                    break;
//                }

                if(PolygonHelper::isPointOnLine(a1 , line) || PolygonHelper::isPointOnLine(a2 , line)||
                   a1 == l1 || a1 == l2 || a2 == l1 || a2 == l2)
                {
                    inBoundry = true;
                    break;
                }
            }
            if(inBoundry)break;
        }

        diffCentroid.emplace_back(diff + (inBoundry*area), i);
    }
    sort(diffCentroid.begin(), diffCentroid.end());

    int centerPolygon = diffCentroid[0].second;
    queue<int> greensQ;
    greensQ.push(centerPolygon);

    set<int> greenAreasIndex;

    while (greenAreas-- )
    {
        int index = diffCentroid[greenAreas].second;
        greenAreasIndex.insert(index);
        lands[index].setDivisible(false);
    }
}
