//
// Created by ahmed Ibrahim on 22-Feb-25.
//

#include "LandDivisionRoads.h"

int LandDivisionRoads::COUNT = 0;

Point LandDivisionRoads::getIntersectionPoint(const Point &p1, double slope, const Line &line) {

    double slope2 = PolygonHelper::getSlope(line.getX1() , line.getX2()  , line.getY1() , line.getY2());


    double x = (line.getY1() - p1.getY() + (slope * p1.getX()) - (slope2 * line.getX1())) / (slope - slope2);

    double y = line.getY1() + slope2 * (x-line.getX1());

    if(abs(line.getY1() - line.getY2()) <= 0.1)y = line.getY1();
    if(abs(line.getX1() - line.getX2()) <= 0.1)x = line.getX1();

    if(x < min(line.getX1() , line.getX2()) || x > max(line.getX1() , line.getX2())) return {INT_MAX , INT_MAX};
    if(y < min(line.getY1() , line.getY2()) || y > max(line.getY1() , line.getY2())) return {INT_MAX , INT_MAX};


    return {x,y};
}

Point LandDivisionRoads::getIntersectionPoint(const Line& line1, const Line& line2, const Point& pointOnLine1) {
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

vector<pair<Polygon1 , Polygon1>> LandDivisionRoads::dividePolygons(Polygon1 polygon1 , double scalingStreet)
{
    COUNT ++;

//    double streetWidth1 = 8 * scalingStreet, streetWidth2 = 6 * scalingStreet, streetWidth3 = 4* scalingStreet;
    double streetWidth1 = 15, streetWidth2 = 15, streetWidth3 = 15;

    vector<pair<Polygon1 , Polygon1>> ans;
    int n = (int)polygon1.getPoints().size();

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

            double x1 = line.getX1() , y1 = line.getY1() ,
            x2 = line.getX2() , y2 = line.getY2();

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

            Line intersectionLine(a3.getX() , a3.getY() , a4.getX() , a4.getY());

            pair<Polygon1  , Polygon1> newTwoPolygons = PolygonHelper::splitPolygons(polygon1 , intersectionLine);

            double area1 = newTwoPolygons.first.getArea();
            double area2 = newTwoPolygons.second.getArea();

            if(min(area1 , area2) / max(area1 , area2) < 0.5) continue;

            ans.emplace_back(newTwoPolygons.first , newTwoPolygons.second);
            break;

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


    for(auto &solution : ans2)
    {
        Polygon1 &pol1 = solution.first;
        Polygon1 &pol2 = solution.second;

        Line roadLine(INT_MAX,INT_MAX,INT_MAX,INT_MAX);
        set<Point> pol1Points;
        for(auto &p : pol1.getPoints()) pol1Points.insert(p);

        for(auto &p : pol2.getPoints())
        {
            if (pol1Points.count(p))
            {
                if (roadLine.getX1() == INT_MAX)
                {
                    roadLine.setX1(p.getX());
                    roadLine.setY1(p.getY());
                }else{
                    roadLine.setX2(p.getX());
                    roadLine.setY2(p.getY());
                }
            }
        }

        Point p1 (roadLine.getX1() , roadLine.getY1());
        Point p2 (roadLine.getX2() , roadLine.getY2());

        // 0.01 * roadLine.getLength()
        double streetWidth = COUNT == 1 ? streetWidth1 : COUNT == 2 ? streetWidth2 : COUNT == 3 ?streetWidth2: streetWidth3;

//        cout<< "streetWidth = "<<streetWidth<<"\n";
        roadLine.print();

        Point center1 = pol1.calculateCentroid();
        Point center2 = pol2.calculateCentroid();

        vector<Point> points1 = pol1.getPoints();
        vector<Point> points2 = pol2.getPoints();

        for (int i = 0; i < points1.size(); ++i)
        {
            Point a0 = points1[i==0?points1.size()-1 : i-1];
            Point a1 = points1[i];
            Point a2 = points1[(i + 1) % points1.size()];
            Point a3 = points1[(i + 2) % points1.size()];
            Point &p = points1[i];
            Point &pp = points1[(i + 1) % points1.size()];

            if ((a1 == p1 && a2 == p2) || (a1 == p2 && a2 == p1))
            {
                Line prev (a0.getX(),a0.getY(),a1.getX(),a1.getY());
                Line next (a2.getX(),a2.getY(),a3.getX(),a3.getY());

                p = shiftPointOnLine(prev , p , -streetWidth);
                pp = shiftPointOnLine(next , pp , streetWidth);
                break;
            }
        }

        for (int i = 0; i < points2.size(); ++i)
        {
            Point a0 = points2[i==0?points2.size()-1 : i-1];
            Point a1 = points2[i];
            Point a2 = points2[(i + 1) % points2.size()];
            Point a3 = points2[(i + 2) % points2.size()];
            Point &p = points2[i];
            Point &pp = points2[(i + 1) % points2.size()];

            if ((a1 == p1 && a2 == p2) || (a1 == p2 && a2 == p1))
            {
                Line prev (a0.getX(),a0.getY(),a1.getX(),a1.getY());
                Line next (a2.getX(),a2.getY(),a3.getX(),a3.getY());

                p = shiftPointOnLine(prev , p , -streetWidth);
                pp = shiftPointOnLine(next , pp , streetWidth);
                break;
            }
        }

        pol1.setPoints(points1);
        pol2.setPoints(points2);

    }
    return ans2;
}

// Function to shift a point along a line by a given distance
Point LandDivisionRoads::shiftPointOnLine(const Line& line, const Point& p, double distance) {
    // Compute the direction vector of the line
    double dx = line.getX2() - line.getX1();
    double dy = line.getY2() - line.getY1();

    // Compute the magnitude (length) of the direction vector
    double length = line.getLength();

    // Normalize the direction vector
    dx /= length;
    dy /= length;

    // Scale by the distance
    dx *= distance;
    dy *= distance;

    // Compute the new point position
    return {p.getX() + dx, p.getY() + dy};
}

double LandDivisionRoads::findY(double x, double x1, double y1, double x2, double y2) {
    // Calculate the slope (m)
    double m = (y2 - y1) / (x2 - x1);

    // Calculate y using y = mx + b
    double y = m * (x - x1) + y1;

    return y;
}

double LandDivisionRoads::findX(double y, double x1, double y1, double x2, double y2) {
    // Calculate the slope (m)
    double m = (y2 - y1) / (x2 - x1);

    // Calculate x using x = (y - b) / m
    double x = (y - y1) / m + x1;

    return x;
}

Polygon1 LandDivisionRoads::buildOutsideRoads(Polygon1 &mainLand)
{
    vector<Point> newPoints;
    vector<Point> curPoints = mainLand.getPoints();

    for (int i = 0; i < curPoints.size(); ++i)
    {
        Point a0 = curPoints[i==0?curPoints.size()-1 : i-1];
        Point a1 = curPoints[i];
        Point a2 = curPoints[(i + 1) % curPoints.size()];

        // Compute the direction vectors of adjacent edges
        Point v1 = (a1 - a0).normalize(); // Edge from a0 to a1
        Point v2 = (a2 - a1).normalize(); // Edge from a1 to a2

        // Compute perpendicular vectors
        Point perp1 = v1.perpendicular();
        Point perp2 = v2.perpendicular();

        // Compute outward direction (average of two perpendicular vectors)
        Point offsetDir = (perp1 + perp2).normalize();

        // Move the point outward by X
        Point newPoint = a1 + offsetDir * 20;

        Line ll(a1.getX() , a1.getY() , newPoint.getX() , newPoint.getY());

        newPoints.push_back(newPoint);
    }

    Polygon1 polygon1(newPoints);

    return polygon1;
}

Polygon1 LandDivisionRoads::getOuterLand(Polygon1 &mainLand)
{
    return buildOutsideRoads(mainLand);
}

LandDivisionRoads::LandDivisionRoads(double divisionArea, double externalRoad, double centralRoad,
                                     double circularStreet, double landDepth, double streetCut) : divisionArea(
        divisionArea), externalRoad(externalRoad), centralRoad(centralRoad), circularStreet(circularStreet),
                                                                                                  landDepth(landDepth),
                                                                                                  streetCut(
                                                                                                          streetCut) {}
