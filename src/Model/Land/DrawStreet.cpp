//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    centerLines = PolygonHelper::getCenterLines(polygon1);

    double step1 = 20 , step2 = 10;

    for (int i = 0; i < centerLines.size(); ++i)
    {
        Line line = centerLines[i];

        Point current = {line.getX1() , line.getY1()};
        Point destination = {line.getX2() , line.getY2()};

        Point next1 = PolygonHelper::getNextPoint(current , destination , step1);
        Point next1UP = {next1.getX() , next1.getY() + 30};
        Point next2 = PolygonHelper::getNextPoint(next1 , destination , step1);
        Point next2UP = {next2.getX() , next2.getY() + 30};

        vector<Line> homeLines;
        homeLines.emplace_back(next1.getX() , next1.getY() , next2.getX() , next2.getY());
        homeLines.emplace_back(next1.getX() , next1.getY() , next1UP.getX() , next1UP.getY());
        homeLines.emplace_back(next2.getX() , next2.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        streets.push_back(homeLines);
    }
}

const vector<Line> &DrawStreet::getCenterLines() const {
    return centerLines;
}

const vector<vector<Line>> &DrawStreet::getStreets() const {
    return streets;
}


