//
// Created by ahmed Ibrahim on 29-Jan-25.
//

#include "PolygonAdjacencyAnalyzer.h"
#include "PolygonHelper.h"
#include "src/Model/DesignGeometryManager.h"


//vector<vector<int>> PolygonAdjacencyAnalyzer::getAdj(vector<Polygon1> &polygons)
//{
//    vector<vector<int>> adj(polygons.size());
//
//    map<Line , vector<int>> lineCnt;
//    vector<set<int>> adjSet(polygons.size());
//
//    for (int i = 0; i < polygons.size(); ++i)
//    {
//        int n = (int)polygons[i].getPoints().size();
//        for (int w = i+1; w < polygons.size(); ++w)
//        {
//            int m = (int)polygons[w].getPoints().size();
//
//            for (int j = 0; j < n; ++j)
//            {
//                Point p1 = polygons[i].getPoints()[j];
//                Point p2 = polygons[i].getPoints()[(j+1) % n];
//
//                Line l1(p1.getX() , p1.getY() , p2.getX() , p2.getY());
//                Line l2(p2.getX() , p2.getY() , p1.getX() , p1.getY());
//
//                for (int k = 0; k < m; ++k) {
//                    Point p3 = polygons[w].getPoints()[k];
//                    Point p4 = polygons[w].getPoints()[(k+1) % m];
//
//                    Line l3(p3.getX() , p3.getY() , p4.getX() , p4.getY());
//                    Line l4(p4.getX() , p4.getY() , p3.getX() , p3.getY());
//
//                    Point intersectLine = PolygonHelper::getIntersectionPoint(l1 , l3);
//
//
//                    bool isLineInsideLine = DesignGeometryManager::doSegmentsIntersect(p1 , p2 , p3 , p4) || l1 == l3 || l1 == l4 || l2 == l3 || l2 == l4
//                                            || (p2 != p3 && p2 != p4 && PolygonHelper::isPointInSegment(p2 , {p3.getX() , p3.getY() , p4.getX() , p4.getY() })) ||
//                                            (p1 != p3 && p1 != p4 && PolygonHelper::isPointInSegment(p1 , {p3.getX() , p3.getY() , p4.getX() , p4.getY() })) ||
//                                            (p3 != p1 && p3 != p2 && PolygonHelper::isPointInSegment(p3 , {p1.getX() , p1.getY() , p2.getX() , p2.getY() })) ||
//                                            (p4 != p1 && p4 != p2 && PolygonHelper::isPointInSegment(p4 , {p1.getX() , p1.getY() , p2.getX() , p2.getY() }));
//
//                    if (isLineInsideLine)
//                    {
//                        adjSet[i].insert(w);
//                        adjSet[w].insert(i);
//
//                        cout<<"Land --> "<<i<<" "<<w<<"\n";
//                    }
//                }
//            }
//        }
//
//    }
//
//
//    for (int i = 0; i < polygons.size(); ++i)
//    {
//        for(auto &tst : adjSet[i])adj[i].push_back(tst);
//
//        cout<<"Land = "<<i+1<<" Size = "<<adj[i].size()<<"\n";
//    }
//    return adj;
//}



vector<vector<int>> PolygonAdjacencyAnalyzer::getAdj(vector<Polygon1> &polygons)
{
    vector<vector<int>> adj(polygons.size());

    map<Line , vector<int>> lineCnt;
    vector<set<int>> adjSet(polygons.size());

    for (int i = 0; i < polygons.size(); ++i)
    {
        int n = (int)polygons[i].getPoints().size();
        for (int w = i+1; w < polygons.size(); ++w)
        {
            int m = (int)polygons[w].getPoints().size();

            for (int j = 0; j < n; ++j)
            {
                Point p1 = polygons[i].getPoints()[j];
                Point p2 = polygons[i].getPoints()[(j+1) % n];

                Line l1(p1.getX() , p1.getY() , p2.getX() , p2.getY());
                Line l2(p2.getX() , p2.getY() , p1.getX() , p1.getY());

                for (int k = 0; k < m; ++k) {
                    Point p3 = polygons[w].getPoints()[k];
                    Point p4 = polygons[w].getPoints()[(k+1) % m];

                    Line l3(p3.getX() , p3.getY() , p4.getX() , p4.getY());
                    Line l4(p4.getX() , p4.getY() , p3.getX() , p3.getY());

                    Point intersectLine = PolygonHelper::getIntersectionPoint(l1 , l3);


                    bool isLineInsideLine = DesignGeometryManager::doSegmentsIntersect(p1 , p2 , p3 , p4) || l1 == l3 || l1 == l4 || l2 == l3 || l2 == l4
                                            || (p2 != p3 && p2 != p4 && PolygonHelper::isPointOnLine(p2 , {p3.getX() , p3.getY() , p4.getX() , p4.getY() })) ||
                                            (p1 != p3 && p1 != p4 && PolygonHelper::isPointOnLine(p1 , {p3.getX() , p3.getY() , p4.getX() , p4.getY() })) ||
                                            (p3 != p1 && p3 != p2 && PolygonHelper::isPointOnLine(p3 , {p1.getX() , p1.getY() , p2.getX() , p2.getY() })) ||
                                            (p4 != p1 && p4 != p2 && PolygonHelper::isPointOnLine(p4 , {p1.getX() , p1.getY() , p2.getX() , p2.getY() }));

                    if (isLineInsideLine)
                    {
                        adjSet[i].insert(w);
                        adjSet[w].insert(i);
                    }
                }
            }
        }

    }


    for (int i = 0; i < polygons.size(); ++i)
    {
        for(auto &tst : adjSet[i])adj[i].push_back(tst);

        cout<<"Land = "<<i+1<<" Size = "<<adj[i].size()<<"\n";
    }
    return adj;
}
