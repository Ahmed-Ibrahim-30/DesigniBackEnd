//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    centerLines = PolygonHelper::getCenterLines(polygon1 , 20);

    vector<Line> centerLinesTop , centerLinesBottom;

    for (int i = 0; i < centerLines.size(); ++i)
    {
        if (i<centerLines.size()/2)centerLinesTop.push_back(centerLines[i]);
        else centerLinesBottom.push_back(centerLines[i]);
    }

    vector<Line> topPoints = PolygonHelper::getTopLines(polygon1 , 20);
    vector<Line> bottomPoints = PolygonHelper::getBottomLines(polygon1 , 20);

    double step1 = 40 ;
    vector<Line> polygonLines = polygon1.getLines();

//    vector<vector<Line>> topStreets = drawTopStreets(polygonLines , topPoints , step1);
//    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines , bottomPoints , step1);
//
//    streets.insert(streets.end() , bottomStreets.begin() , bottomStreets.end());
//    streets.insert(streets.end() , topStreets.begin() , topStreets.end());

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
        vector<Line> bottomLines;

        Point startPoint = getNextPoint(lastPoint , centerLineIndex , centerLines , step , bottomLines);

        if (startPoint.getX() == INT_MAX) break;
        bottomLines.clear();

        lastPoint = getNextPoint(startPoint , centerLineIndex , centerLines , step , bottomLines);

        if (lastPoint.getX() == INT_MAX) break;

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
        }

        vector<Line> homeLines;
        homeLines.insert(homeLines.end() , bottomLines.begin() , bottomLines.end());
        homeLines.emplace_back(startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());
        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        topStreets.push_back(homeLines);

        //EXTENSIONS
        vector<Line> extensions = drawExtensions(polygonLines , bottomLines , startPoint , lastPoint , next1UP , next2UP , step/2 , true);

        CityGrid cityGrid;
        cityGrid.setStreets(homeLines);
        cityGrid.setRoadExtension(extensions);
        cities.push_back(cityGrid);

        //Home border
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

        vector<Line> topLines;

        Point startPoint = getNextPoint(lastPoint , centerLineIndex , centerLines , newStep , topLines);

        topLines.clear();

        if (startPoint.getX() == INT_MAX)break;


        lastPoint = getNextPoint(startPoint , centerLineIndex , centerLines , step , topLines);

        if (lastPoint.getX() == INT_MAX) break;

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
        }

        vector<Line> homeLines;
        homeLines.insert(homeLines.end() , topLines.begin() , topLines.end());
        homeLines.emplace_back(startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());
        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        bottomStreets.push_back(homeLines);



        //Extensions
        vector<Line> extensions = drawExtensions(polygonLines , topLines , startPoint , lastPoint , next1UP , next2UP , step/2 , false);

        CityGrid cityGrid;
        cityGrid.setStreets(homeLines);
        cityGrid.setRoadExtension(extensions);
        cities.push_back(cityGrid);
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

vector<Line> DrawStreet::drawExtensions(const vector<Line> &polygonLines ,const vector<Line> &topLines , const Point &start, const Point &end, const Point &startUp, const Point &endUp,double step , bool isTop)
{
    vector<Line> extensions;
    Point centerBottom(100,100) , centerTop;
    double reqLength = step;

    for(auto &bLine : topLines)
    {
        double lineLen = bLine.getLength();

        if (lineLen < reqLength)
        {
            reqLength -= lineLen;
            continue;
        }

        centerBottom = PolygonHelper::getNextPoint({bLine.getX1() , bLine.getY1()} , {bLine.getX2() , bLine.getY2()} , reqLength);
        break;
    }
    cout<<"centerBottom = "<<centerBottom.getX()<<" "<<centerBottom.getY()<<"\n";

    centerTop = Point ((startUp.getX()+endUp.getX())/2 , (startUp.getY()+endUp.getY())/2);

    Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
    extensions.push_back(centerExtension);

    int lineIndex1 = 0 , lineIndex2 = 0;

    for (int i = 0; i < centerLines.size(); ++i) {
        Line c = centerLines[i];

        double minX = min(c.getX1() , c.getX2());
        double maxX = max(c.getX1() , c.getX2());
        double minY = min(c.getY1() , c.getY2());
        double maxY = max(c.getY1() , c.getY2());

        if (start.getX() >= minX && start.getX() <= maxX &&
        start.getY() >= minY && start.getY() <= maxY)
        {
            lineIndex1 = i;
        }
        if (end.getX() >= minX && end.getX() <= maxX &&
                end.getY() >= minY && end.getY() <= maxY)
        {
            lineIndex2 = i;
        }
    }
    vector<Line>tt;
    Point nextEnd = getNextPoint(end , lineIndex2 , centerLines , step , tt);
    Point prevStart = getPrevPoint(start , lineIndex1 , centerLines , step , tt);


    Line rightExtension (nextEnd.getX() , nextEnd.getY() , nextEnd.getX() , nextEnd.getY() + (1000000000 * (isTop? 1 :-1)));
    Line leftExtension (prevStart.getX() , prevStart.getY() , prevStart.getX() , prevStart.getY() + (1000000000 * (isTop? 1 :-1)));

    for(auto &pLine : polygonLines)
    {
        Point intersection = PolygonHelper::getIntersectionPoint(pLine , rightExtension);

        if (intersection.getX() != INT_MAX)
        {
            rightExtension.setY2(intersection.getY());
            break;
        }
    }
    extensions.push_back(rightExtension);

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

    return extensions;
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
            connLines.emplace_back(startPoint.getX() , startPoint.getY() , endPoint.getX() , endPoint.getY());
            break;
        }
    }
    return endPoint;
}

Point DrawStreet::getPrevPoint(const Point &end, int &lineIndex, const vector<Line> &lines, double step ,vector<Line> &connLines)
{
    Point startPoint , endPoint = end;
    while (true)
    {
        const Line& line = lines[lineIndex];
        Point destination = {line.getX1() , line.getY1()};
        Line partialLine {endPoint.getX() , endPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length < step)
        {
            lineIndex --;
            if (lineIndex < 0)
            {
                return {INT_MAX, INT_MAX};
            }
            step -= length;
            endPoint = {line.getX1() , line.getY1()};
        }
        else
        {
            startPoint = PolygonHelper::getNextPoint(endPoint , destination , step);
            break;
        }
    }
    return startPoint;
}


const vector<Line> &DrawStreet::getCenterLines() const {
    return centerLines;
}

const vector<vector<Line>> &DrawStreet::getStreets() const {
    return streets;
}

const vector<CityGrid> &DrawStreet::getCities() const {
    return cities;
}














