//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    centerLines = PolygonHelper::getCenterLines(polygon1);

    vector<Line> topPoints = PolygonHelper::getTopLines(polygon1 , 20);
    vector<Line> bottomPoints = PolygonHelper::getBottomLines(polygon1 , 20);

    double step1 = 40 , step2 = 30;
    vector<Line> polygonLines = polygon1.getLines();

    vector<vector<Line>> topStreets = drawTopStreets(polygonLines , topPoints , step1);
    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines , bottomPoints , step1);

    streets.insert(streets.end() , bottomStreets.begin() , bottomStreets.end());

//    for (int i = 0; i < centerLines.size(); ++i)
//    {
//        Line line = centerLines[i];
//
//        Point current(line.getX1() , line.getY1());
//        step2 = 30;
//
//        while (true)
//        {
//            Point destination = {line.getX2() , line.getY2()};
//
//            Point next1 = PolygonHelper::getNextPoint(current , destination , step2);
//
//            if (next1 == destination){
//                break;
//            }
//            bool foundIntersection = false;
//
//            Point next1UP = {next1.getX() , next1.getY() - height};
//
//            Line nextLine (next1.getX() , next1.getY() , next1UP.getX() , next1UP.getY());
//
//            for(auto &upLine : bottomPoints)
//            {
//                Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);
//
//                if (intersection.getX() != INT_MAX)
//                {
//                    foundIntersection = true;
//                    next1UP = intersection;
//                    break;
//                }
//            }
//
//            if (!foundIntersection)
//            {
//                for(auto &pLine : polygonLines)
//                {
//                    Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);
//
//                    if (intersection.getX() != INT_MAX)
//                    {
//                        next1UP = intersection;
//                        break;
//                    }
//                }
//            }
//
//            Point next2 = PolygonHelper::getNextPoint(next1 , destination , step1);
//
//            if (next2 == destination){
//                break;
//            }
//
//            Point next2UP = {next2.getX() , next2.getY() - height};
//
//            nextLine = Line(next2.getX() , next2.getY() , next2UP.getX() , next2UP.getY());
//
//            foundIntersection = false;
//            for(auto &upLine : bottomPoints)
//            {
//                Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);
//
//                if (intersection.getX() != INT_MAX)
//                {
//                    foundIntersection = true;
//                    next2UP = intersection;
//                    break;
//                }
//            }
//
//            if (!foundIntersection)
//            {
//                for(auto &pLine : polygonLines)
//                {
//                    Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);
//
//                    if (intersection.getX() != INT_MAX)
//                    {
//                        next2UP = intersection;
//                        break;
//                    }
//                }
//            }
//
//            vector<Line> homeLines;
//            homeLines.emplace_back(next1.getX() , next1.getY() , next2.getX() , next2.getY());
//            homeLines.emplace_back(next1.getX() , next1.getY() , next1UP.getX() , next1UP.getY());
//            homeLines.emplace_back(next2.getX() , next2.getY() , next2UP.getX() , next2UP.getY());
//            homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());
//
//            streets.push_back(homeLines);
//
//            step2 = 20;
//            current = next2;
//        }
//    }

//    streets.push_back(topPoints);
//    streets.push_back(bottomPoints);
}

vector<vector<Line>> DrawStreet::drawTopStreets(const vector<Line> &polygonLines, const vector<Line> &topLine, double step)
{
    vector<vector<Line>> topStreets;
    double height = 100000000000;

    int centerLineIndex = 0;
    Point lastPoint = {centerLines[0].getX1() , centerLines[0].getY1() };
    while(centerLineIndex < centerLines.size())
    {
        Point startPoint = {0 , 0};

        Line line = centerLines[centerLineIndex];

        vector<Line> bottomLines;

        Point destination = {line.getX2() , line.getY2()};

        Line partialLine {lastPoint.getX() , lastPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length < step)
        {
            centerLineIndex ++;
            if (centerLineIndex == centerLines.size())break;
            line = centerLines[centerLineIndex];
            destination = {line.getX2() , line.getY2()};
            startPoint =  PolygonHelper::getNextPoint({line.getX1() , line.getY1()} , destination , step - length);
        }else startPoint = PolygonHelper::getNextPoint(lastPoint , destination , step);

        partialLine ={startPoint.getX() , startPoint.getY() , destination.getX() , destination.getY()};
        length = partialLine.getLength();

        if (length < step)
        {
            bottomLines.push_back(partialLine);

            centerLineIndex ++;
            if (centerLineIndex == centerLines.size())break;
            line = centerLines[centerLineIndex];
            destination = {line.getX2() , line.getY2()};
            lastPoint =  PolygonHelper::getNextPoint({line.getX1() , line.getY1()} , destination , step - length);

            bottomLines.emplace_back(line.getX1() , line.getY1() , lastPoint.getX() , lastPoint.getY());
        }else
        {
            lastPoint = PolygonHelper::getNextPoint(startPoint , destination , step);
            bottomLines.emplace_back(startPoint.getX() , startPoint.getY() , lastPoint.getX() , lastPoint.getY());
        }


        Point next1UP = {startPoint.getX() , startPoint.getY() + height};

        Line nextLine (startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());

        bool foundIntersection = false;
        for(auto &upLine : topLine)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next1UP = intersection;
                break;
            }
        }
        if (!foundIntersection)
        {
            for(auto &pLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    next1UP = intersection;
                    break;
                }
            }
        }


        Point next2UP = {lastPoint.getX() , lastPoint.getY() + height};

        foundIntersection = false;
        nextLine = Line(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        for(auto &upLine : topLine)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next2UP = intersection;
                break;
            }
        }
        if (!foundIntersection)
        {
            for(auto &pLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    next2UP = intersection;
                    break;
                }
            }
        }

        vector<Line> homeLines;
        homeLines.insert(homeLines.end() , bottomLines.begin() , bottomLines.end());
        homeLines.emplace_back(startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());
        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        topStreets.push_back(homeLines);
    }
    return topStreets;
}

vector<vector<Line>> DrawStreet::drawBottomStreets(const vector<Line> &polygonLines, const vector<Line> &bottomLines, double step)
{
    vector<vector<Line>> bottomStreets;
    double height = 100000000000;

    int centerLineIndex = 0 ;
    Point lastPoint = {centerLines[0].getX1() , centerLines[0].getY1() };
    while(centerLineIndex < centerLines.size())
    {
        double newStep = bottomStreets.empty() ? step + step/2 : step;
        Point startPoint = {0 , 0};

        Line line = centerLines[centerLineIndex];

        vector<Line> topLines;

        Point destination = {line.getX2() , line.getY2()};

        Line partialLine {lastPoint.getX() , lastPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length < newStep)
        {
            centerLineIndex ++;
            if (centerLineIndex == centerLines.size())break;
            line = centerLines[centerLineIndex];
            destination = {line.getX2() , line.getY2()};
            startPoint =  PolygonHelper::getNextPoint({line.getX1() , line.getY1()} , destination , newStep - length);
        }else startPoint = PolygonHelper::getNextPoint(lastPoint , destination , newStep);

        partialLine ={startPoint.getX() , startPoint.getY() , destination.getX() , destination.getY()};
        length = partialLine.getLength();

        if (length < step)
        {
            topLines.push_back(partialLine);

            centerLineIndex ++;
            if (centerLineIndex == centerLines.size())break;
            line = centerLines[centerLineIndex];
            destination = {line.getX2() , line.getY2()};
            lastPoint =  PolygonHelper::getNextPoint({line.getX1() , line.getY1()} , destination , step - length);

            topLines.emplace_back(line.getX1() , line.getY1() , lastPoint.getX() , lastPoint.getY());
        }else
        {
            lastPoint = PolygonHelper::getNextPoint(startPoint , destination , step);
            topLines.emplace_back(startPoint.getX() , startPoint.getY() , lastPoint.getX() , lastPoint.getY());
        }


        Point next1UP = {startPoint.getX() , startPoint.getY() - height};

        Line nextLine (startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());

        bool foundIntersection = false;
        for(auto &upLine : bottomLines)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next1UP = intersection;
                break;
            }
        }
        if (!foundIntersection)
        {
            for(auto &pLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    next1UP = intersection;
                    break;
                }
            }
        }


        Point next2UP = {lastPoint.getX() , lastPoint.getY() - height};

        foundIntersection = false;
        nextLine = Line(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        for(auto &upLine : bottomLines)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next2UP = intersection;
                break;
            }
        }
        if (!foundIntersection)
        {
            for(auto &pLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(pLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    next2UP = intersection;
                    break;
                }
            }
        }

        vector<Line> homeLines;
        homeLines.insert(homeLines.end() , topLines.begin() , topLines.end());
        homeLines.emplace_back(startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());
        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        bottomStreets.push_back(homeLines);
    }
    return bottomStreets;
}

const vector<Line> &DrawStreet::getCenterLines() const {
    return centerLines;
}

const vector<vector<Line>> &DrawStreet::getStreets() const {
    return streets;
}






