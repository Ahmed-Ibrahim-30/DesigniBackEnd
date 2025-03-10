//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    centerLines = PolygonHelper::getCenterLines(polygon1);

    vector<Line> topPoints = PolygonHelper::getTopLines(polygon1 , 20);
    vector<Line> bottomPoints = PolygonHelper::getBottomLines(polygon1 , 20);

    double step1 = 40 ;
    vector<Line> polygonLines = polygon1.getLines();

    vector<vector<Line>> topStreets = drawTopStreets(polygonLines , topPoints , step1);
    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines , bottomPoints , step1);

    streets.insert(streets.end() , bottomStreets.begin() , bottomStreets.end());
    streets.insert(streets.end() , topStreets.begin() , topStreets.end());

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
            continue;
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
            continue;
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

        vector<Line> extensions;
        Point centerBottom , centerTop;
        double reqLength = step/2;
        for(auto &bLine : bottomLines)
        {
            double lineLen = bLine.getLength();

            if (lineLen < reqLength)
            {
                reqLength -= lineLen;
                continue;
            }

            centerBottom = PolygonHelper::getNextPoint({bLine.getX1() , bLine.getY1()} , {bLine.getX2() , bLine.getY2()} , reqLength);
        }

        centerTop = Point ((next1UP.getX()+next2UP.getX())/2 , (next1UP.getY()+next2UP.getY())/2);

        Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
        extensions.push_back(centerExtension);

        Line leftExtension (startPoint.getX() , startPoint.getY() , startPoint.getX() , startPoint.getY() - height);

        for(auto &pLine : polygonLines)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(pLine , leftExtension);

            if (intersection.getX() != INT_MAX)
            {
                leftExtension.setY2(intersection.getY());
                break;
            }
        }
        extensions.push_back(leftExtension);
        roadExtension.push_back(extensions);

        //Home border
        vector<Line> homeBorderSol;

        Point start = startPoint;
        Point last = lastPoint;
        Point center = centerBottom;

        while (true)
        {
            Point nextPoint1 = PolygonHelper::getNextPoint(start , next1UP , 20);
            Point nextPoint2 = PolygonHelper::getNextPoint(last , next2UP , 20);
            Point nextPoint3 = PolygonHelper::getNextPoint(center , centerTop , 20);

            if (nextPoint1 == next1UP || nextPoint2 == next2UP || nextPoint3 == centerTop)
            {
                break;
            }

            homeBorderSol.emplace_back(nextPoint1.getX() , nextPoint1.getY() , nextPoint3.getX() , nextPoint3.getY());
            homeBorderSol.emplace_back(nextPoint2.getX() , nextPoint2.getY() , nextPoint3.getX() , nextPoint3.getY());

            start = nextPoint1;
            last = nextPoint2;
            center = nextPoint3;
        }
        homeBorder.push_back(homeBorderSol);
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

        Line line = centerLines[centerLineIndex];

        vector<Line> topLines;

        Point destination = {line.getX2() , line.getY2()};

        Point startPoint = getNextPoint(lastPoint , centerLineIndex , centerLines , newStep , topLines);

        topLines.clear();

        if (startPoint.getX() == INT_MAX)break;


        lastPoint = getNextPoint(startPoint , centerLineIndex , centerLines , step , topLines);

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
            continue;
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
            continue;
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


        //Extensions
//        vector<Line> extensions;
//        Point centerBottom , centerTop;
//        double reqLength = step/2;
//        for(auto &bLine : topLines)
//        {
//            double lineLen = bLine.getLength();
//
//            if (lineLen < reqLength)
//            {
//                reqLength -= lineLen;
//                continue;
//            }
//
//            centerBottom = PolygonHelper::getNextPoint({bLine.getX1() , bLine.getY1()} , {bLine.getX2() , bLine.getY2()} , reqLength);
//        }
//
//        centerTop = Point ((next1UP.getX()+next2UP.getX())/2 , (next1UP.getY()+next2UP.getY())/2);
//
//        Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
//        extensions.push_back(centerExtension);
//
//
//        Line rightExtension (lastPoint.getX() , lastPoint.getY() , lastPoint.getX() , lastPoint.getY() + height);
//
//        for(auto &pLine : polygonLines)
//        {
//            Point intersection = PolygonHelper::getIntersectionPoint(pLine , rightExtension);
//
//            if (intersection.getX() != INT_MAX)
//            {
//                rightExtension.setY2(intersection.getY());
//                break;
//            }
//        }
//        extensions.push_back(rightExtension);
//
//        roadExtension.push_back(extensions);


        //Home Border

//        vector<Line> homeBorderSol;
//
//        Point start = startPoint;
//        Point last = lastPoint;
//        Point center = centerBottom;
//
//        while (true)
//        {
//            Point nextPoint1 = PolygonHelper::getNextPoint(start , next1UP , 20);
//            Point nextPoint2 = PolygonHelper::getNextPoint(last , next2UP , 20);
//            Point nextPoint3 = PolygonHelper::getNextPoint(center , centerTop , 20);
//
//            if (nextPoint1 == next1UP || nextPoint2 == next2UP || nextPoint3 == centerTop)
//            {
//                break;
//            }
//
//            homeBorderSol.emplace_back(nextPoint1.getX() , nextPoint1.getY() , nextPoint3.getX() , nextPoint3.getY());
//            homeBorderSol.emplace_back(nextPoint2.getX() , nextPoint2.getY() , nextPoint3.getX() , nextPoint3.getY());
//
//            start = nextPoint1;
//            last = nextPoint2;
//            center = nextPoint3;
//        }
//        homeBorder.push_back(homeBorderSol);
    }
    return bottomStreets;
}

Point DrawStreet::getNextPoint(const Point &start, int &lineIndex, const vector<Line> &lines, double step ,vector<Line> &connLines)
{
    Point endPoint , startPoint = start;
    while (true)
    {
        const Line& line = lines[lineIndex];
        Point destination = {line.getX2() , line.getY2()};
        Line partialLine {startPoint.getX() , startPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length < step)
        {
            connLines.push_back(partialLine);
            lineIndex ++;
            if (lineIndex == lines.size())
            {
                return {INT_MAX, INT_MAX};
            }
            step -= length;
            startPoint = {line.getX2() , line.getY2()};
        }
        else
        {
            endPoint = PolygonHelper::getNextPoint(startPoint , destination , step);
            connLines.emplace_back(line.getX1() , line.getY1() , endPoint.getX() , endPoint.getY());
            break;
        }
    }
    return endPoint;
}

const vector<Line> &DrawStreet::getCenterLines() const {
    return centerLines;
}

const vector<vector<Line>> &DrawStreet::getStreets() const {
    return streets;
}

const vector<vector<Line>> &DrawStreet::getRoadExtension() const {
    return roadExtension;
}

const vector<vector<Line>> &DrawStreet::getHomeBorder() const {
    return homeBorder;
}








