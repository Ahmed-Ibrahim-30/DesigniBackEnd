//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#include "PolygonsSubtraction.h"

PolygonsSubtraction::PolygonsSubtraction(const Polygon1 &mainPolygon, const Polygon1 &secondPolygon) : mainPolygon(
        mainPolygon), secondPolygon(secondPolygon) {}

vector<Polygon1> PolygonsSubtraction::getNewPolygons()
{

    vector<Line> mainPolygonLines = mainPolygon.getLines();
    vector<Line> lines = mainPolygonLines;
    Point maxP = mainPolygon.maxPoint() , minP = mainPolygon.minPoint();

    vector<Line> secondPolygonLines = secondPolygon.getLines();

    Point centroid = mainPolygon.calculateCentroid();

    double scale = max({maxP.getX() - centroid.getX() , centroid.getX() - minP.getX() ,
                       maxP.getY() - centroid.getY() , centroid.getY() - minP.getY()});

    for(auto &line : secondPolygonLines)
    {
        Line extendLine = PolygonHelper::extendLine(line , true , scale * 10);

        for(int i  = 0; i< lines.size() ; i++)
        {
            Line mainL = lines[i];
            Point intersectionPoint = PolygonHelper::getIntersectionPoint(extendLine , mainL);

            if (intersectionPoint.getX() != INT_MAX)
            {
                Line line1(mainL.getX1() , mainL.getY1() , intersectionPoint.getX() , intersectionPoint.getY());
                Line line2(mainL.getX2() , mainL.getY2() , intersectionPoint.getX() , intersectionPoint.getY());
                Line line3(extendLine.getX1() , extendLine.getY1() , intersectionPoint.getX() , intersectionPoint.getY());
                Line line4(extendLine.getX2() , extendLine.getY2() , intersectionPoint.getX() , intersectionPoint.getY());

                lines.erase(lines.begin() + i);

                lines.push_back(line1);
                lines.push_back(line2);

                if(extendLine.getX1() >= minP.getX() && extendLine.getX1() <= maxP.getX()
                        && extendLine.getY1() >= minP.getY() && extendLine.getY1() <= maxP.getY())
                {
                    lines.push_back(line3);
                }

                if(extendLine.getX2() >= minP.getX() && extendLine.getX2() <= maxP.getX()
                        && extendLine.getY2() >= minP.getY() && extendLine.getY2() <= maxP.getY())
                {
                    lines.push_back(line4);
                }
                // lines.push_back(line3);
                // lines.push_back(line4);
                break;
            }
        }
    }
    lines.insert(lines.end() , secondPolygonLines.begin() , secondPolygonLines.end());

    LineToPolygonConverter lineToPolygonConverter(lines);
    vector<Polygon1> newPolygons = lineToPolygonConverter.constructPolygons(secondPolygon);

    set<Point> pointsInner;
    for(auto &p : secondPolygon.getPoints())pointsInner.insert(p);

    vector<Polygon1>ans;

    for(auto &pol : newPolygons) {
        set<Point> points2;
        for(auto &p : pol.getPoints())points2.insert(p);

        if(points2 == pointsInner)continue;
        ans.push_back(pol);
        // pol.print();
    }

    secondPolygon = Polygon1(secondPolygon.getPoints() , false);
    ans.push_back(secondPolygon);

    // cout<<"Inner Polygons\n";
    // vector<Polygon1> ppps = ans;
    // for(auto &pol : ppps) {
    //     cout<<"***********\n";
    //     pol.shiftX(400);
    //     pol.shiftY(400);
    //     pol.print();
    // }

    return ans;

}
