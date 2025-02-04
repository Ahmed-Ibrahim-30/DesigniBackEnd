//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#include "LandNetworkBySpacing.h"

// vector<Polygon1> LandNetworkBySpacing::buildLandNetwork(const Polygon1 &mainPolygon, const vector<Polygon1> &polys) {
//     vector<Polygon1> pol = polys;

//     double spacing = 2.8 , spacing2 = 1.07;

//     map<int , map<Point, Point>> mapPolygonPoints;

//     Polygon1 mainPol = mainPolygon;
//     vector<Point> points3 =mainPol.scalePolygon(spacing2);
//     mainPol.setPoints(points3);

//     for(int p1 = 0; p1 < pol.size() ; p1++)
//     {
//         int index1 = p1;
//         Polygon1 c1 = pol[index1];
//         vector<Point> points1 = c1.getPoints();

//         Point maxPoint1 = c1.maxPoint();

//         for (int p2 = p1+1; p2 < pol.size(); ++p2)
//         {
//             int index2 = p2;
//             Polygon1 c2 = pol[index2];
//             vector<Point> points2 = c2.getPoints();

//             Point maxPoint2 = c2.maxPoint();

//             for (int i = 0; i < points1.size(); ++i)
//             {
//                 Point a1 = points1[i];
//                 Point a2 = points1[(i+1)%points1.size()];

//                 Point a10 = points1[i == 0 ? points1.size()-1 : i-1];

//                 Point a21 = points1[(i+2) % points1.size()];

//                 double x1 = a1.getX() , x2 = a2.getX();
//                 double y1 = a1.getY() , y2 = a2.getY();

//                 Line prev(a10.getX() , a10.getY() , a1.getX() , a1.getY());
//                 Line next(a2.getX() , a2.getY() , a21.getX() , a21.getY());

//                 double topSlope = getSlope(prev.getX1() , prev.getX2() , prev.getY1() , prev.getY2());

//                 double bottomSlope = getSlope(next.getX1() , next.getX2() , next.getY1() , next.getY2());

//                 Point a1Replace(a1.getX() , a1.getY() - spacing);

//                 Point a2Replace(a2.getX() , a2.getY() - spacing);


//                 for (int j = 0; j < points2.size(); ++j)
//                 {
//                     Point a3 = points2[j];
//                     Point a4 = points2[(j+1)%points2.size()];

//                     Point a30 = points2[j == 0 ? points2.size()-1 : j-1];

//                     Point a41 = points2[(j+2) % points2.size()];


//                     Line prev2(a30.getX() , a30.getY() , a3.getX() , a3.getY());
//                     Line next2(a4.getX() , a4.getY() , a41.getX() , a41.getY());


//                     double topSlope2 = getSlope(prev2.getX1() , prev2.getX2() , prev2.getY1() , prev2.getY2());

//                     double bottomSlope2 = getSlope(next2.getX1() , next2.getX2() , next2.getY1() , next2.getY2());


//                     Point a3Replace(a3.getX() , a3.getY() - spacing);

//                     Point a4Replace(a4.getX() , a4.getY() - spacing);

//                     bool isLineInsideLine = (a1 == a3 && a2 == a4) || (a1 == a4 && a2 == a3)
//                                             || (a2 != a3 && a2 != a4 && isPointInSegment(a2 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
//                                             (a1 != a3 && a1 != a4 && isPointInSegment(a1 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
//                                             (a3 != a1 && a3 != a2 && isPointInSegment(a3 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() })) ||
//                                             (a4 != a1 && a4 != a2 &&isPointInSegment(a4 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() }));

//                     if(isLineInsideLine)
//                     {


//                         double maxX2 = max(a3.getX() , a4.getX());
//                         double minX2 = min(a3.getX() , a4.getX());

//                         double maxY2 = max(a3.getY() , a4.getY());
//                         double minY2 = min(a3.getY() , a4.getY());

//                         if(max({x2 , x1}) - min(x2,x1) >= max(y2 , y1) - min(y2,y1)
//                            &&  maxX2 - minX2 >= maxY2 - minY2)
//                         {
//                             // cout<<"Land -- >"<<index1 <<" "<<index2<<"\n";

//                             //first Polygon is above Line
//                             if(maxPoint1.getY() < maxPoint2.getY())
//                             {
//                                 if(!mapPolygonPoints[index1].count(a1) || !mapPolygonPoints[index1].count(a2))
//                                 {
//                                     a1Replace = Point(a1.getX() , a1.getY() - spacing);
//                                     a2Replace = Point(a2.getX() , a2.getY() - spacing);

//                                     if(topSlope != 0)
//                                     {
//                                         a1Replace.setX(((a1Replace.getY() - a1.getY() )/topSlope) + a1.getX());
//                                     }

//                                     if(bottomSlope != 0)
//                                     {
//                                         a2Replace.setX(((a2Replace.getY() - a2.getY() )/bottomSlope) + a2.getX());
//                                     }

//                                     if(!mapPolygonPoints[index1].count(a1))mapPolygonPoints[index1][a1] = a1Replace;
//                                     if(!mapPolygonPoints[index1].count(a2))mapPolygonPoints[index1][a2] = a2Replace;

//                                 }

//                                 if(!mapPolygonPoints[index2].count(a3) || !mapPolygonPoints[index2].count(a4))
//                                 {
//                                     a3Replace = Point(a3.getX() , a3.getY() + spacing);
//                                     a4Replace = Point(a4.getX() , a4.getY() + spacing);

//                                     if(topSlope2 != 0)
//                                     {
//                                         a3Replace.setX(((a3Replace.getY() - a3.getY() )/topSlope2) + a3.getX());
//                                     }

//                                     if(bottomSlope2 != 0)
//                                     {
//                                         a4Replace.setX(((a4Replace.getY() - a4.getY() )/bottomSlope2) + a4.getX());
//                                     }
//                                     if(!mapPolygonPoints[index2].count(a3))mapPolygonPoints[index2][a3] = a3Replace;
//                                     if(!mapPolygonPoints[index2].count(a4))mapPolygonPoints[index2][a4] = a4Replace;
//                                 }
//                             }
//                             else
//                             {
//                                 if(!mapPolygonPoints[index1].count(a1) || !mapPolygonPoints[index1].count(a2))
//                                 {
//                                     a1Replace = Point(a1.getX() , a1.getY() + spacing);
//                                     a2Replace = Point(a2.getX() , a2.getY() + spacing);

//                                     if(topSlope != 0)
//                                     {
//                                         a1Replace.setX(((a1Replace.getY() - a1.getY() )/topSlope) + a1.getX());
//                                     }

//                                     if(bottomSlope != 0)
//                                     {
//                                         a2Replace.setX(((a2Replace.getY() - a2.getY() )/bottomSlope) + a2.getX());
//                                     }
//                                     if(!mapPolygonPoints[index1].count(a1))mapPolygonPoints[index1][a1] = a1Replace;
//                                     if(!mapPolygonPoints[index1].count(a2))mapPolygonPoints[index1][a2] = a2Replace;
//                                 }

//                                 if(!mapPolygonPoints[index2].count(a3) || !mapPolygonPoints[index2].count(a4))
//                                 {
//                                     a3Replace = Point(a3.getX() , a3.getY() - spacing);
//                                     a4Replace = Point(a4.getX() , a4.getY() - spacing);

//                                     if(topSlope2 != 0)
//                                     {
//                                         a3Replace.setX(((a3Replace.getY() - a3.getY() )/topSlope2) + a3.getX());
//                                     }

//                                     if(bottomSlope2 != 0)
//                                     {
//                                         a4Replace.setX(((a4Replace.getY() - a4.getY() )/bottomSlope2) + a4.getX());
//                                     }
//                                     if(!mapPolygonPoints[index2].count(a3))mapPolygonPoints[index2][a3] = a3Replace;
//                                     if(!mapPolygonPoints[index2].count(a4))mapPolygonPoints[index2][a4] = a4Replace;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }

//     }

//     for(int p1 = 0; p1 < pol.size() ; p1++)
//     {
//         vector<Point> points;
//         for(auto &p : pol[p1].getPoints())
//         {
//             if(mapPolygonPoints[p1].count(p))
//             {
//                 points.push_back( mapPolygonPoints[p1][p]);
//             }
//             else
//             {
//                 points.push_back(p);
//             }
//         }
//         // pol[p1].setPoints(points);
//      }

//     mapPolygonPoints.clear();

//     for(int p1 = 0; p1 < pol.size() ; p1++)
//     {
//         if(p1 > 3) break;
//         int index1 = p1;
//         Polygon1 c1 = pol[index1];
//         vector<Point> points1 = c1.getPoints();

//         Point maxPoint1 = c1.maxPoint();

//         for (int p2 = p1+1; p2 < pol.size(); ++p2)
//         {
//             int index2 = p2;



//             Polygon1 c2 = pol[index2];
//             vector<Point> points2 = c2.getPoints();

//             Point maxPoint2 = c2.maxPoint();

//             for (int i = 0; i < points1.size(); ++i)
//             {
//                 Point a1 = points1[i];
//                 Point a2 = points1[(i+1)%points1.size()];



//                 Point a10 = points1[i == 0 ? points1.size()-1 : i-1];

//                 Point a21 = points1[(i+2) % points1.size()];

//                 double x1 = a1.getX() , x2 = a2.getX();
//                 double y1 = a1.getY() , y2 = a2.getY();

//                 Line prev(a10.getX() , a10.getY() , a1.getX() , a1.getY());
//                 Line next(a2.getX() , a2.getY() , a21.getX() , a21.getY());

//                 double topSlope = round(getSlope(prev.getX1() , prev.getX2() , prev.getY1() , prev.getY2()) * 10) / 10;

//                 double bottomSlope = round(getSlope(next.getX1() , next.getX2() , next.getY1() , next.getY2()) * 10) / 10;

//                 Point a1Replace(a1.getX() , a1.getY() - spacing);

//                 Point a2Replace(a2.getX() , a2.getY() - spacing);

//                 for (int j = 0; j < points2.size(); ++j)
//                 {
//                     Point a3 = points2[j];
//                     Point a4 = points2[(j+1)%points2.size()];

//                     Point a30 = points2[j == 0 ? points2.size()-1 : j-1];

//                     Point a41 = points2[(j+2) % points2.size()];


//                     Line prev2(a30.getX() , a30.getY() , a3.getX() , a3.getY());
//                     Line next2(a4.getX() , a4.getY() , a41.getX() , a41.getY());

//                     if(p1 == 3 && p2 == 28 && (a1 == a3 && a2 == a4) || (a1 == a4 && a2 == a3))
//                     {
//                         cout<<"DEBUG\n";
//                     }


//                     double topSlope2 = round(getSlope(prev2.getX1() , prev2.getX2() , prev2.getY1() , prev2.getY2()) ) ;

//                     double bottomSlope2 = round(getSlope(next2.getX1() , next2.getX2() , next2.getY1() , next2.getY2()) );


//                     Point a3Replace(a3.getX() , a3.getY() - spacing);

//                     Point a4Replace(a4.getX() , a4.getY() - spacing);


//                     bool isLineInsideLine = (a1 == a3 && a2 == a4) || (a1 == a4 && a2 == a3)
//                                             || (a2 != a3 && a2 != a4 && isPointInSegment(a2 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
//                                             (a1 != a3 && a1 != a4 && isPointInSegment(a1 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
//                                             (a3 != a1 && a3 != a2 && isPointInSegment(a3 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() })) ||
//                                             (a4 != a1 && a4 != a2 &&isPointInSegment(a4 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() }));

//                     if(isLineInsideLine)
//                     {

//                         double maxX2 = max(a3.getX() , a4.getX());
//                         double minX2 = min(a3.getX() , a4.getX());

//                         double maxY2 = max(a3.getY() , a4.getY());
//                         double minY2 = min(a3.getY() , a4.getY());

//                         if(max({x2 , x1}) - min(x2,x1) <= max(y2 , y1) - min(y2,y1)
//                            &&  maxX2 - minX2 <= maxY2 - minY2)
//                         {
//                             //first Polygon is Left Line
//                             if(maxPoint1.getX() < maxPoint2.getX())
//                             {
//                                 if(!mapPolygonPoints[index1].count(a1) && !mapPolygonPoints[index1].count(a2))
//                                 {
//                                     a1Replace = Point(a1.getX() - spacing, a1.getY() );
//                                     a2Replace = Point(a2.getX() - spacing, a2.getY() );

//                                     if(topSlope != 0)
//                                     {
//                                         a1Replace .setY(((a1Replace.getX() - a1.getX() )*topSlope) + a1.getY());
//                                     }

//                                     if(bottomSlope != 0)
//                                     {
//                                         a2Replace .setY(((a2Replace.getX() - a2.getX() )*bottomSlope) + a2.getY());
//                                     }

//                                     mapPolygonPoints[index1][a1] = a1Replace;
//                                     mapPolygonPoints[index1][a2] = a2Replace;
//                                 }

//                                 if(!mapPolygonPoints[index2].count(a3) && !mapPolygonPoints[index2].count(a4))
//                                 {
//                                     a3Replace = Point(a3.getX() + spacing, a3.getY() );
//                                     a4Replace = Point(a4.getX() + spacing, a4.getY() );

//                                     if(topSlope2 != 0)
//                                     {
//                                         a3Replace .setY(((a3Replace.getX() - a3.getX() )*topSlope2) + a3.getY());
//                                     }

//                                     if(bottomSlope2 != 0)
//                                     {
//                                         a4Replace .setY(((a4Replace.getX() - a4.getX() )*bottomSlope2) + a4.getY());
//                                     }

//                                     mapPolygonPoints[index2][a3] = a3Replace;
//                                     mapPolygonPoints[index2][a4] = a4Replace;
//                                 }
//                             }
//                             else
//                             {
//                                 if(!mapPolygonPoints[index1].count(a1) && !mapPolygonPoints[index1].count(a2))
//                                 {
//                                     a1Replace = Point(a1.getX() + spacing, a1.getY() );
//                                     a2Replace = Point(a2.getX() + spacing, a2.getY() );

//                                     if(topSlope != 0)
//                                     {
//                                         a1Replace .setY(((a1Replace.getX() - a1.getX() )*topSlope) + a1.getY());
//                                     }

//                                     if(bottomSlope != 0)
//                                     {
//                                         a2Replace .setY(((a2Replace.getX() - a2.getX() )*bottomSlope) + a2.getY());
//                                     }

//                                     mapPolygonPoints[index1][a1] = a1Replace;
//                                     mapPolygonPoints[index1][a2] = a2Replace;
//                                 }

//                                 if(!mapPolygonPoints[index2].count(a3) && !mapPolygonPoints[index2].count(a4))
//                                 {
//                                     a3Replace = Point(a3.getX() - spacing, a3.getY() );
//                                     a4Replace = Point(a4.getX() - spacing, a4.getY() );

//                                     if(topSlope2 != 0)
//                                     {
//                                         a3Replace .setY(((a3Replace.getX() - a3.getX() )*topSlope2) + a3.getY());
//                                     }

//                                     if(bottomSlope2 != 0)
//                                     {
//                                         a4Replace .setY(((a4Replace.getX() - a4.getX() )*bottomSlope2) + a4.getY());
//                                     }

//                                     mapPolygonPoints[index2][a3] = a3Replace;
//                                     mapPolygonPoints[index2][a4] = a4Replace;
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }


//     }

//     for(int p1 = 0; p1 < pol.size() ; p1++)
//     {
//         vector<Point> points;
//         for(auto &p : pol[p1].getPoints())
//         {
//             if(mapPolygonPoints[p1].count(p))
//             {
//                 points.push_back( mapPolygonPoints[p1][p]);
//             }
//             else
//             {
//                 points.push_back(p);
//             }
//         }
//         pol[p1].setPoints(points);
//     }

//     cout<<"Final output = "<<"\n";
//     for (auto & var : pol)
//     {
//         // if(var == 5) break;
//         var.print();

//         // if(var == 1) break;
//     }
//     mainPol.print();

//     pol.push_back(mainPol);
//     return pol;
// }




vector<Polygon1> LandNetworkBySpacing::buildLandNetwork(const Polygon1 &mainPolygon, const vector<Polygon1> &polys)
{

    double spacing = 2.8 , spacing2 = 1.07;

    vector<Polygon1> pol = polys , old = polys;

    map<int , map<Point , shiftingDirection>> linesShiftingDir = setShiftDirection(pol);

    map<int , map<Point , bool>> isPointShared = storePointsShared(pol);

    for(int p1 = 0; p1 < pol.size() ; p1++)
    {
        Polygon1 c1 = pol[p1];
        vector<Point> points1 = c1.getPoints();
        vector<Point> points2 = old[p1].getPoints();

        Point centroid = c1.calculateCentroid();

        for (int i = 0; i < points1.size(); ++i)
        {
            Point aOld = points2[i];
            Point &a1 = points1[i];
            Point &a2 = points1[(i+1)%points1.size()];

            Point &a4 = points1[i == 0 ? points1.size()-1 : i-1];

            Point &a3 = points1[(i+2) % points1.size()];

            Line prev(a4.getX() , a4.getY() , a1.getX() , a1.getY());
            Line next(a2.getX() , a2.getY() , a3.getX() , a3.getY());

            double topSlope = getSlope(prev.getX1() , prev.getX2() , prev.getY1() , prev.getY2());

            double bottomSlope = getSlope(next.getX1() , next.getX2() , next.getY1() , next.getY2());

            Point a1Replace(a1.getX() , a1.getY() - spacing);

            Point a2Replace(a2.getX() , a2.getY() - spacing);

            if(isPointShared[p1][aOld])
            {
                shiftingDirection dir = linesShiftingDir[p1][a1];

                if(dir == shiftingDirection::TOP)
                {
                    a1Replace = Point(a1.getX() , a1.getY() - spacing);
                    a2Replace = Point(a2.getX() , a2.getY() - spacing);

                    if(topSlope != 0)
                    {
                        a1Replace.setX(((a1Replace.getY() - a1.getY() )/topSlope) + a1.getX());
                    }

                    if(bottomSlope != 0)
                    {
                        a2Replace.setX(((a2Replace.getY() - a2.getY() )/bottomSlope) + a2.getX());
                    }

                    a1 = a1Replace;
                    a2 = a2Replace;
                }

                else if(dir == shiftingDirection::BOTTOM)
                {
                    a1Replace = Point(a1.getX() , a1.getY() + spacing);
                    a2Replace = Point(a2.getX() , a2.getY() + spacing);

                    if(topSlope != 0)
                    {
                        a1Replace.setX(((a1Replace.getY() - a1.getY() )/topSlope) + a1.getX());
                    }

                    if(bottomSlope != 0)
                    {
                        a2Replace.setX(((a2Replace.getY() - a2.getY() )/bottomSlope) + a2.getX());
                    }

                    a1 = a1Replace;
                    a2 = a2Replace;
                }

                else if(dir == shiftingDirection::RIGHT)
                {
                    a1Replace = Point(a1.getX() + spacing, a1.getY() );
                    a2Replace = Point(a2.getX() + spacing, a2.getY() );

                    if(topSlope != 0)
                    {
                        a1Replace .setY(((a1Replace.getX() - a1.getX() )*topSlope) + a1.getY());
                    }

                    if(bottomSlope != 0)
                    {
                        a2Replace .setY(((a2Replace.getX() - a2.getX() )*bottomSlope) + a2.getY());
                    }

                    a1 = a1Replace;
                    a2 = a2Replace;
                }

                else if(dir == shiftingDirection::LEFT)
                {
                    a1Replace = Point(a1.getX() - spacing, a1.getY() );
                    a2Replace = Point(a2.getX() - spacing, a2.getY() );

                    if(topSlope != 0)
                    {
                        a1Replace .setY(((a1Replace.getX() - a1.getX() )*topSlope) + a1.getY());
                    }

                    if(bottomSlope != 0)
                    {
                        a2Replace .setY(((a2Replace.getX() - a2.getX() )*bottomSlope) + a2.getY());
                    }

                    a1 = a1Replace;
                    a2 = a2Replace;
                }
            }

        }
        pol[p1].setPoints(points1);
    }



    Polygon1 mainPol = mainPolygon;
    vector<Point> points3 =mainPol.scalePolygon(spacing2);
    mainPol.setPoints(points3);

    cout<<"Final output = "<<"\n";
    for (auto & var : pol)
    {
        // if(var == 5) break;
        var.print();

        // if(var == 1) break;
    }

    mainPol.print();

    pol.push_back(mainPol);
    return pol;
}


map<int , map<Point , bool>> LandNetworkBySpacing::storePointsShared(const vector<Polygon1> &polys)
{
    map<int , map<Point , bool>> isPointShared;
    vector<Polygon1> pol = polys;

    for(int p1 = 0; p1 < pol.size() ; p1++)
    {
        Polygon1 c1 = pol[p1];
        vector<Point> points1 = c1.getPoints();

        for (int p2 = p1+1; p2 < pol.size(); ++p2)
        {
            int index2 = p2;
            Polygon1 c2 = pol[index2];
            vector<Point> points2 = c2.getPoints();

            for (int i = 0; i < points1.size(); ++i)
            {
                Point a1 = points1[i];
                Point a2 = points1[(i+1)%points1.size()];


                for (int j = 0; j < points2.size(); ++j)
                {
                    Point a3 = points2[j];
                    Point a4 = points2[(j+1)%points2.size()];

                    bool isLineInsideLine = (a1 == a3 && a2 == a4) || (a1 == a4 && a2 == a3)
                                            || (a2 != a3 && a2 != a4 && isPointInSegment(a2 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
                                            (a1 != a3 && a1 != a4 && isPointInSegment(a1 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
                                            (a3 != a1 && a3 != a2 && isPointInSegment(a3 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() })) ||
                                            (a4 != a1 && a4 != a2 &&isPointInSegment(a4 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() }));

                    if(isLineInsideLine)
                    {
                        isPointShared[p1][a1] = true;
                        isPointShared[p2][a3] = true;
                    }
                }
            }
        }

    }

    return isPointShared;
}


map<int , map<Point , LandNetworkBySpacing::shiftingDirection>>LandNetworkBySpacing::setShiftDirection(const vector<Polygon1> &polys)
{
    map<int , map<Point , shiftingDirection>> linesShiftingDir;

    vector<Polygon1> pol = polys;

    for(int p1 = 0; p1 < pol.size() ; p1++)
    {
        Polygon1 c1 = pol[p1];
        vector<Point> points1 = c1.getPoints();
        Point centroid = c1.calculateCentroid();

        for (int i = 0; i < points1.size(); ++i)
        {
            Point a1 = points1[i];
            Point a2 = points1[(i+1)%points1.size()];

            Point a4 = points1[i == 0 ? points1.size()-1 : i-1];

            Point a3 = points1[(i+2) % points1.size()];

            Point midP((a1.getX() + a2.getX()) / 2 , (a1.getY() + a2.getY()) / 2);

            double dx = midP.getX() - centroid.getX();
            double dy = midP.getY() - centroid.getY();

            if (fabs(dx) > fabs(dy)) {
                if (dx > 0) linesShiftingDir[p1][a1] = shiftingDirection::RIGHT ;
                else linesShiftingDir[p1][a1] = shiftingDirection::LEFT;
            } else {
                if (dy > 0) linesShiftingDir[p1][a1] = shiftingDirection::TOP ;
                else linesShiftingDir[p1][a1] = shiftingDirection::BOTTOM;
            }

            continue;


            if(a3.getY() < a2.getY() && a4.getY() < a2.getY() )
            {
                linesShiftingDir[p1][a1] = shiftingDirection::TOP;
            }
            else if(a3.getY() > a2.getY() && a4.getY() > a1.getY())
            {
                linesShiftingDir[p1][a1] = shiftingDirection::BOTTOM;
            }
            else if( a3.getX() < a2.getX()&& a4.getX() < a1.getX() )
            {
                linesShiftingDir[p1][a1] = shiftingDirection::LEFT;
            }
            else if( a3.getX() > a2.getX()&& a4.getX() > a1.getX())
            {
                linesShiftingDir[p1][a1] = shiftingDirection::RIGHT;
            }

        }
    }

    return linesShiftingDir;
}
