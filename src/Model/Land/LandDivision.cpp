//
// Created by ahmed Ibrahim on 15-Jan-25.
//
#include "LandDivision.h"

// double LandDivision::getSlope(double x1, double x2, double y1, double y2) {

//     if(x1 == x2)return 0;
//     return (y2-y1) / (x2 - x1);
// }

// bool LandDivision::isPointInSegment(const Point &p, const Line &line)
// {

//     double slope = getSlope(line.getX1() , line.getX2() ,line.getY1() , line.getY2());

//     double slopeWithPoint = getSlope(p.getX() , line.getX1() , p.getY() , line.getY1());

//     slope = MathUtils::roundingToDecimal(slope);
//     slopeWithPoint = MathUtils::roundingToDecimal(slopeWithPoint);

//     double epsilon = 0.1;

//     if(slope != INT_MAX && slopeWithPoint!= INT_MAX && abs(slopeWithPoint- slope) > epsilon)
//     {
//         return false;
//     }
//     if(p.getX() < min(line.getX1() , line.getX2()) || p.getX() > max(line.getX1() , line.getX2())) return false;
//     if(p.getY() < min(line.getY1() , line.getY2()) || p.getY() > max(line.getY1() , line.getY2())) return false;

//     return true;
// }

Point LandDivision::getIntersectionPoint(const Point &p1, double slope, const Line &line) {

    double slope2 = PolygonHelper::getSlope(line.getX1() , line.getX2()  , line.getY1() , line.getY2());


    double x = (line.getY1() - p1.getY() + (slope * p1.getX()) - (slope2 * line.getX1())) / (slope - slope2);

    double y = line.getY1() + slope2 * (x-line.getX1());

    if(abs(line.getY1() - line.getY2()) <= 0.1)y = line.getY1();
    if(abs(line.getX1() - line.getX2()) <= 0.1)x = line.getX1();

    if(x < min(line.getX1() , line.getX2()) || x > max(line.getX1() , line.getX2())) return {INT_MAX , INT_MAX};
    if(y < min(line.getY1() , line.getY2()) || y > max(line.getY1() , line.getY2())) return {INT_MAX , INT_MAX};


    return {x,y};
}

Point LandDivision::getIntersectionPoint(const Line& line1, const Line& line2, const Point& pointOnLine1) {
    // Calculate the parametric value t for the point on the first line
    double t;
    if (line1.getX1() != line1.getX2()) { // Non-vertical line
        t = (pointOnLine1.getX() - line1.getX1()) / (line1.getX2() - line1.getX1());
    } else { // Vertical line
        t = (pointOnLine1.getY() - line1.getY1()) / (line1.getY2() - line1.getY1());
    }

    Point pointOnLine2;
    pointOnLine2.setX( line2.getX1() + t * (line2.getX2()- line2.getX1()) );
    pointOnLine2.setY( line2.getY1() + t * (line2.getY2() - line2.getY1()) );

    return pointOnLine2;
}


vector<pair<Polygon1 , Polygon1>> LandDivision::dividePolygons(Polygon1 polygon1)
{

    vector<pair<Polygon1 , Polygon1>> ans;
    int n = polygon1.getPoints().size();

    set<Line> polLines;

    vector<pair<double , Line>> allLines;

    Point maxPolygonPoint = polygon1.maxPoint();
    Point minPolygonPoint = polygon1.minPoint();

    double ratioA = 0 , ratioB = 0;

    for (int i = 0; i < n; ++i) {
        Point a1 = polygon1.getPoints()[i];
        Point a2 = polygon1.getPoints()[(i+1)%n];

        polLines.emplace(a1.getX() , a1.getY() , a2.getX() , a2.getY());

        double x1 = a1.getX() , y1 = a1.getY() , x2 = a2.getX() , y2 = a2.getY();

        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );

        allLines.emplace_back(length , Line(x1 , y1 , x2 , y2));
    }

    sort(allLines.begin() , allLines.end() , greater<>());

    vector<pair<double , double>> ratios = {{1 , 1} , {1 , 2} , {2 , 1} , {3 , 2} , {1 , 3} , {2,5} , {1 , 4} , {3,4} , {4,3}};

    for(auto &rat : ratios)
    {
        ratioA = rat.first;
        ratioB = rat.second;

        for (auto &l : allLines)
        {
            Line line = l.second;

            double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

            Point a1(x1 , y1);
            Point a2(x2 , y2);

            double slope = PolygonHelper::getSlope(x1 , x2 ,y1 , y2);

            double pX = 0 , pY = 0;

            if(a1.getX() >= a2.getX())
            {
                pX = a1.getX() - (a1.getX() - a2.getX()) * (ratioB/(ratioA+ratioB));
            }
            else
            {
                pX = a1.getX() + (a2.getX() - a1.getX() ) * (ratioB/(ratioA+ratioB));
            }

            if(a1.getY() >= a2.getY())
            {
                pY = a1.getY() - (a1.getY() - a2.getY()) * (ratioB/(ratioA+ratioB));
            }
            else
            {
                pY = a1.getY() + (a2.getY() - a1.getY() ) * (ratioB/(ratioA+ratioB));
            }

            Point a3 (pX , pY);
            // Point a4 (INT_MAX,INT_MAX);

            Point a4 (pX * (maxPolygonPoint.getX() - pX + 100),pY * (maxPolygonPoint.getY() - pY + 100));

            for (int j = 0; j < n; j++) {
                Point p1 = polygon1.getPoints()[j];
                Point p2 = polygon1.getPoints()[(j + 1) % n];

                Line line1 (p1.getX() , p1.getY() , p2.getX() , p2.getY());
                Line line2 (p2.getX() , p2.getY() , p1.getX() , p1.getY());

                // if(p1 == a1 || p2 == a1 || p1 == a2 || p2==a2) continue;
                if(line1 == line || line2 == line) continue;

                Point intersectionPoint(0,0);

                if(abs(a1.getX() - a2.getX()) > 0.1 && abs(a1.getY() - a2.getY()) > 0.1) intersectionPoint = getIntersectionPoint(a3 , -1/slope , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});


                else{
                    if(abs(a1.getX() - a2.getX()) <= 0.1)
                    {
                        a4 = Point(pX * ((maxPolygonPoint.getX() - minPolygonPoint.getX()) *10) , a3.getY());
                    }
                    else
                    {
                        a4 = Point(a3.getX() , pY * ((maxPolygonPoint.getY() - minPolygonPoint.getY()) *10));
                    }
                    intersectionPoint = PolygonHelper::getIntersectionPoint({a3.getX() , a3.getY() , a4.getX() , a4.getY()} , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});

                    if(intersectionPoint.getX() == INT_MAX || (intersectionPoint != a1 && intersectionPoint != a2))
                    {
                        if(abs(a1.getX() - a2.getX()) <= 0.1)
                        {
                            a4 = Point(pX * -((maxPolygonPoint.getX() - minPolygonPoint.getX()) *10) , a3.getY());
                        }
                        else
                        {
                            a4 = Point(a3.getX() , pY * -((maxPolygonPoint.getY() - minPolygonPoint.getY()) *10));
                        }
                        intersectionPoint = PolygonHelper::getIntersectionPoint({a3.getX() , a3.getY() , a4.getX() , a4.getY()} , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});
                    }
                }



                if(intersectionPoint.getX() != INT_MAX && intersectionPoint != a1 && intersectionPoint != a2)
                {
                    a4 = intersectionPoint;
                    break;
                }
            }
            if(a4.getX() != INT_MAX)
            {
                bool flag = false;
                set<Line> newPolygonLine = polLines;

                for(auto &pLine : newPolygonLine)
                {
                    if(PolygonHelper::isPointInSegment(a3 , pLine))
                    {
                        Line p = pLine;
                        newPolygonLine.erase(pLine);
                        newPolygonLine.emplace(a3.getX() , a3.getY() , p.getX1() , p.getY1());
                        newPolygonLine.emplace(a3.getX() , a3.getY() , p.getX2() , p.getY2());
                        flag = true;
                        break;
                    }
                }

                if(!flag) continue;
                flag = false;
                for(auto &pLine : newPolygonLine)
                {
                    if(PolygonHelper::isPointInSegment(a4 , pLine))
                    {
                        Line p = pLine;
                        newPolygonLine.erase(pLine);
                        newPolygonLine.emplace(a4.getX() , a4.getY() , p.getX1() , p.getY1());
                        newPolygonLine.emplace(a4.getX() , a4.getY() , p.getX2() , p.getY2());
                        flag = true;
                        break;
                    }
                }


                if(!flag) continue;
                newPolygonLine.emplace(a3.getX() , a3.getY() , a4.getX() , a4.getY());

                vector<Point> p1s , p2s;
                a3.setX(MathUtils::roundingToDecimal(a3.getX()));
                a3.setY(MathUtils::roundingToDecimal(a3.getY()));

                a4.setX(MathUtils::roundingToDecimal(a4.getX()));
                a4.setY(MathUtils::roundingToDecimal(a4.getY()));

                p1s.push_back(a3);
                Point curPoint = a3;
                while(curPoint != a4)
                {
                    flag = false;
                    for(auto &pLine : newPolygonLine)
                    {
                        Point p1(pLine.getX1() , pLine.getY1());
                        Point p2(pLine.getX2() , pLine.getY2());

                        if( (p1== a4  || p2 == a4) && p1s.size()==1) continue;

                        if(p1 == curPoint)
                        {
                            p1s.push_back(p2);
                            curPoint = p2;
                            newPolygonLine.erase(pLine);
                            flag = true;
                            break;
                        }
                        else if(p2 == curPoint)
                        {
                            p1s.push_back(p1);
                            curPoint = p1;
                            newPolygonLine.erase(pLine);
                            flag = true;
                            break;
                        }
                    }
                    if(!flag) break;
                }

                if(!flag) continue;
                p2s.push_back(a3);
                curPoint = a3;
                while(curPoint != a4)
                {
                    flag = false;
                    for(auto &pLine : newPolygonLine)
                    {
                        Point p1(pLine.getX1() , pLine.getY1());
                        Point p2(pLine.getX2() , pLine.getY2());

                        if( (p1== a4  || p2 == a4) && p2s.size()==1) continue;

                        if(p1 == curPoint)
                        {
                            p2s.push_back(p2);
                            curPoint = p2;
                            newPolygonLine.erase(pLine);
                            flag = true;
                            break;
                        }
                        else if(p2 == curPoint)
                        {
                            p2s.push_back(p1);
                            curPoint = p1;
                            newPolygonLine.erase(pLine);
                            flag = true;
                            break;
                        }

                    }
                    if(!flag) break;
                }
                if(!flag) continue;

                Polygon1 po1 (p1s) , po2(p2s);

                double area1 = po1.getArea();
                double area2 = po2.getArea();

                if(min(area1 , area2) / max(area1 , area2) < 0.5) continue;


                // po1.print();
                // po2.print();


                ans.emplace_back(po1 , po2);
                break;
                // return {first , second};
            }
        }

    }


    vector<pair<double , int>> sortPols;
    for (int i = 0; i < ans.size(); ++i)
    {
        Polygon1 first  = ans[i].first;
        Polygon1 second = ans[i].second;

        vector<Line> lines = first.getLines();
        double diff = 0;
        for (int j = 0; j < lines.size(); ++j) {
            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
                    x2 = lines[j].getX2() , y2 = lines[j].getY2();

            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();

            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
            diff += abs(length - length2);
        }

        lines = second.getLines();
        for (int j = 0; j < lines.size(); ++j) {
            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
                    x2 = lines[j].getX2() , y2 = lines[j].getY2();

            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();

            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
            diff += abs(length - length2);
        }

        sortPols.emplace_back(diff , i);
    }

    sort(sortPols.begin() , sortPols.end());

    vector<pair<Polygon1 , Polygon1>> ans2;
    for(auto &tst : sortPols)ans2.push_back(ans[tst.second]);


    return ans2;
}

