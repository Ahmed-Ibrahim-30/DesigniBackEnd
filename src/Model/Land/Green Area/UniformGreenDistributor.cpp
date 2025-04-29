//
// Created by ahmed Ibrahim on 30-Jan-25.
//

#include "UniformGreenDistributor.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "src/Model/DesignGeometryManager.h"

//void UniformGreenDistributor::select( Polygon1 &outerLand ,vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas ) {
//    int n = (int)lands.size();
//    int greenAreas = n*greenAreaPercentage;
//    cout<<"GREEN AREA = "<<greenAreas<<"\n";
//    vector<vector<int>> landsAdj = PolygonAdjacencyAnalyzer::getAdj(lands);
//
//    Point outerCentroid = outerLand.calculateCentroid();
//    vector<Line> outerLine = outerLand.getLines();
//
//    vector<set<int>> landsAdjSet(n);
//
//    for (int i = 0; i < landsAdj.size(); ++i)
//    {
//        for(auto &adj : landsAdj[i])landsAdjSet[i].insert(adj);
//    }
//
//    vector<pair<int , int>> mapIndexToAdjCount;
//    map<int , bool> isLandInBoundry;
//    for (int i = 0; i < n; ++i)
//    {
//
//        bool inBoundry = false;
//        double area = lands[i].getArea();
//
//        for (int j = 0; j < lands[i].getPoints().size(); ++j)
//        {
//            Point a1 = lands[i].getPoints()[j];
//            Point a2 = lands[i].getPoints()[(j+1) % lands[i].getPoints().size()];
//
//
//            for(auto &line : outerLine)
//            {
//                Point l1 (line.getX1() , line.getY1());
//                Point l2 (line.getX2() , line.getY2());
//
//                if(PolygonHelper::isPointOnLine(a1 , line)||PolygonHelper::isPointOnLine(a2 , line)||
//                        a1 == l1 || a1 == l2 || a2 == l1 || a2 == l2)
//                {
//                    inBoundry = true;
//                    break;
//                }
//            }
//            if(inBoundry)break;
//        }
//        isLandInBoundry[i] = inBoundry;
//        if(inBoundry)cout<<"outerLand = "<<i+1<<"\n";
//        mapIndexToAdjCount.emplace_back(landsAdj[i].size() + (inBoundry * -area), i);
//    }
//    sort(mapIndexToAdjCount.begin(), mapIndexToAdjCount.end() , greater<>());
//
//    for(auto &line : outerLine)
//    {
//        line.printJsonFormat();
//    }
//
//    set<int> greenAreasIndex;
//
//    while (greenAreas--)
//    {
//        int index = mapIndexToAdjCount[0].second;
//        greenAreasIndex.insert(index);
//        cout<<"INDEX OF GREEN AREA = "<<index <<"Adj Size = "<<mapIndexToAdjCount[0].first<<"\n";
//        lands[index].setDivisible(false);
//
//        set<int> removedIndex;
//        double area = lands[index].getArea();
//        removedIndex.insert(index);
//        // for(auto &ad : landsAdjSet[index])removedIndex.insert(ad);
//
//        mapIndexToAdjCount.clear();
//        landsAdjSet[index].clear();
//        for (int i = 0; i < n; ++i)
//        {
//            set<int> newAdj = landsAdjSet[i];
//            bool neighbourToGreenArea = false;
//            for(auto &adj : landsAdjSet[i])
//            {
//                if (removedIndex.count(adj)){
//                    newAdj.erase(adj);
//                }
//            }
//            for(auto &adj : landsAdj[i])if(greenAreasIndex.count(adj))neighbourToGreenArea = true;
//            landsAdjSet[i] = newAdj;
//
//            if(!greenAreasIndex.count(i))mapIndexToAdjCount.emplace_back(landsAdjSet[i].size() +(neighbourToGreenArea*-area) +(isLandInBoundry[i] *-5*area), i);
//        }
//        sort(mapIndexToAdjCount.begin(), mapIndexToAdjCount.end() , greater<>());
//    }
//}

void UniformGreenDistributor::select( Polygon1 &outerLand ,vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas )
{
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
    cout<<"GREEN AREA = "<<greenAreas<<"\n";

    vector<vector<int>> landsAdj = PolygonAdjacencyAnalyzer::getAdj(lands);

    vector< set<int>> landsAdjS(lands.size());

    for (int i = 0; i < landsAdj.size(); ++i)
    {
        string landID = lands[i].getId();
        for (int j : landsAdj[i])
        {
            landsAdjS[i].insert(j);
        }
    }
    vector<pair<int , int>> priorities;

    for (int i = 0; i < landsAdj.size(); ++i)
    {
        priorities.emplace_back(landsAdj[i].size(),i);
    }


    for (int i = 0; i < greenAreas; ++i)
    {
        sort(priorities.begin(), priorities.end() , greater<>());

        int greenAreaIndex = priorities[0].second;

        lands[greenAreaIndex].setDivisible(false);

        cout<<lands[greenAreaIndex].getId() <<"IS GREEN AREA \n";

        //update Priority
        for (auto & j : priorities) {
            if (landsAdjS[greenAreaIndex].count(j.second))
            {
                j.first--;
                j.first -= 100000;
            }
            else if (j.second == greenAreaIndex)
            {
                j.first  = -10000000 - i;
            }
        }

        for(auto &adj : landsAdjS[greenAreaIndex])
        {
            landsAdjS[adj].erase(greenAreaIndex);
        }
        landsAdjS[greenAreaIndex].clear();
    }
}

//void UniformGreenDistributor::select( Polygon1 &outerLand ,vector<Polygon1> &lands, double greenAreaPercentage, double totalGreenAreas ) {
//    int n = (int)lands.size();
//    int greenCount = n*greenAreaPercentage;
//    cout<<"GREEN AREA = "<<greenCount<<"\n";
//
//    vector<tuple<Polygon1, Point , int>> centroids;
//
//    // Compute centroid for each polygon
//    for (int i = 0; i < lands.size(); ++i) {
//        centroids.emplace_back(lands[i], lands[i].calculateCentroid() , i);
//    }
//
//    // Sort polygons by X and Y centroid positions to spread them evenly
//    std::sort(centroids.begin(), centroids.end(), []( auto& a,  auto& b) {
//        Polygon1 polygon1 , polygon2;
//        Point point1(0,0) , point2(0,0);
//        int index1 , index2;
//        tie(polygon1 , point1 , index1) = a;
//        tie(polygon1 , point1 , index2) = b;
//        return point1.getX() + point1.getY() < point2.getX() + point2.getY();
//    });
//
//    // Select green areas by choosing evenly spaced centroids
//    int step = (int)lands.size() / greenCount;
//    for (int i = 0; i < greenCount; i++) {
//        Polygon1 polygon1 ;
//        Point point1(0,0) ;
//        int index1 ;
//
//        tie(polygon1 , point1 , index1) = centroids[i * step];
//
//        lands[index1].setDivisible(false);
//    }
//}
