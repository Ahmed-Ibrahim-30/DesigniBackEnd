//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    double step1 = 40 ;
    mainLand = polygon1;
    centerLines = PolygonHelper::getCenterLines(polygon1 , 10);

    vector<Line> centerLinesTop , centerLinesBottom;

    for (int i = 0; i < centerLines.size(); ++i)
    {
        if (i<centerLines.size()/2)centerLinesTop.push_back(centerLines[i]);
        else centerLinesBottom.push_back(centerLines[i]);
    }

    vector<Line> topPoints = PolygonHelper::getTopLines(polygon1 , 30);
    vector<Line> bottomPoints = PolygonHelper::getBottomLines(polygon1 , 30);


    int divisions = 0 , index = 0 , divisionsB = 1;

    Point start = PolygonHelper::getNextPoint({centerLinesTop[index].getX1() , centerLinesTop[index].getY1()},{centerLinesTop[index].getX2() , centerLinesTop[index].getY2()},startSpace);

    while (true)
    {
        vector<Line> ll;
        Point next = getNextPoint(start , index , centerLinesTop , step1/2 , ll);
        if (next.getX() == INT_MAX) break;

        Line nextLine(next.getX() , next.getY() , next.getX() , next.getY()+ 10000000);
        bool foundIntersection = false;
        for(auto &upLine : topPoints)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                Line newline = upLine;
                newline.setX1(intersection.getX());
                newline.setY1(intersection.getY());

                if (upLine == topPoints.back() && newline.getLength() <= 10) break;
                foundIntersection = true;
                break;
            }
        }

        if (!foundIntersection) break;
        divisions++;
        start = next;
    }

    divisions/=4;
    index = 0;

    start = PolygonHelper::getNextPoint({centerLinesBottom[index].getX1() , centerLinesBottom[index].getY1()},{centerLinesBottom[index].getX2() , centerLinesBottom[index].getY2()},startSpace + step1/2);

    while (true)
    {
        vector<Line> ll;
        Point next = getNextPoint(start , index , centerLinesBottom , step1/2 , ll);
        if (next.getX() == INT_MAX) break;

        Line nextLine(next.getX() , next.getY() , next.getX() , next.getY()- 10000000);
        bool foundIntersection = false;
        for(auto &upLine : bottomPoints)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                Line newline = upLine;
                newline.setX1(intersection.getX());
                newline.setY1(intersection.getY());

                if (upLine == bottomPoints.back() &&newline.getLength() <= 10) break;
                foundIntersection = true;
                break;
            }
        }

        if (!foundIntersection) break;

        divisionsB++;
        start = next;
    }

    divisionsB/=4;

    cout<<"divisions = "<<divisions <<"\n";
    cout<<"divisionsB = "<<divisionsB <<"\n";

    divisions = min(divisions , divisionsB);

    double lengthC = 0;
    for(auto &line : centerLinesTop)
    {
        lengthC += line.getLength();
    }

    step1 = lengthC / (int)(lengthC/40);
    cout<<"New Step = "<<step1<<"\n";

    lengthC = 0;
    for(auto &line : centerLinesBottom)
    {
        lengthC += line.getLength();
    }

    step1 = min(step1 , lengthC / (int)(lengthC/40));
    cout<<"New Step = "<<step1<<"\n";



    vector<Line> polygonLines = polygon1.getLines();

    vector<vector<Line>> topStreets = drawTopStreets(polygonLines ,centerLinesTop ,  topPoints , step1 , divisions);
    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines ,centerLinesBottom, bottomPoints , step1 , divisions);

//    streets.push_back(topPoints);
//    streets.push_back(bottomPoints);
}



vector<vector<Line>> DrawStreet::drawTopStreets(const vector<Line> &polygonLines,const vector<Line> &centerL , const vector<Line> &topLine, double step ,int divisions)
{
    vector<vector<Line>> topStreets;
    double height = 100000000000;

    int centerLineIndex = 0;
    Point lastPoint = {centerL[0].getX1() , centerL[0].getY1() };
    for (int m = 0; m < divisions; ++m)
    {
        vector<Line> bottomLines , bottomLines2;

        double newStep = topStreets.empty() ? startSpace : step;
        int curIndex = centerLineIndex;

        Point startPoint1 = getNextPoint(lastPoint , centerLineIndex , centerL , newStep , bottomLines);
        Point startPoint2 = getNextPoint(lastPoint , curIndex , centerL , newStep + 5 , bottomLines);

        if (startPoint1.getX() == INT_MAX || startPoint2.getX() == INT_MAX) break;
        bottomLines.clear();

        curIndex = centerLineIndex;

        lastPoint = getNextPoint(startPoint1 , centerLineIndex , centerL , step , bottomLines);
        Point lastPoint2 = getNextPoint(startPoint2 , curIndex , centerL , step-10 , bottomLines2);

        if (lastPoint.getX() == INT_MAX) break;

        Point next1UP = {startPoint1.getX() , startPoint1.getY() + height};

        Line nextLine (startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());

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

        Point next12UP = {startPoint2.getX() , startPoint2.getY() + height};

        nextLine =Line (startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());

        foundIntersection = false;
        for(auto &upLine : topLine)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next12UP = intersection;
                next12UP.setY(next12UP.getY() - 5);
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

        Point next22UP = {lastPoint2.getX() , lastPoint2.getY() + height};

        foundIntersection = false;
        nextLine = Line(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        for(auto &upLine : topLine)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next22UP = intersection;
                next22UP.setY(next22UP.getY() - 5);
                break;
            }
        }
        if (!foundIntersection)
        {
            continue;
        }

        vector<Line> homeLinesOuter , homeLinesInner;
        homeLinesOuter.insert(homeLinesOuter.end() , bottomLines.begin() , bottomLines.end());
        homeLinesOuter.emplace_back(startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());
        homeLinesOuter.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLinesOuter.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        topStreets.push_back(homeLinesOuter);
        homeLinesInner.insert(homeLinesInner.end() , bottomLines2.begin() , bottomLines2.end());
        homeLinesInner.emplace_back(startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());
        homeLinesInner.emplace_back(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        homeLinesInner.emplace_back(next12UP.getX() , next12UP.getY() , next22UP.getX() , next22UP.getY());


        //EXTENSIONS And Border
        vector<Line> extensions = drawExtensions(polygonLines , bottomLines2 , startPoint2 , lastPoint2 , next12UP , next22UP , step/2 , true, centerL);
        //vector<Line> homeBorder = drawHomeBorders( mainLand, homeLines , extensions , true);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
        cityGrid.setRoadExtension(extensions);
//        cityGrid.setHomeBorder(homeBorder);

        cities.push_back(cityGrid);
    }
    return topStreets;
}

vector<vector<Line>> DrawStreet::drawBottomStreets(const vector<Line> &polygonLines,const vector<Line> &centerL , const vector<Line> &bottomLines, double step ,int divisions)
{
    vector<vector<Line>> bottomStreets;
    double height = 100000000000;

    int centerLineIndex = 0 ;
    Point lastPoint = {centerL[0].getX1() , centerL[0].getY1() };
    for (int m = 0; m < divisions; ++m)
    {
        double newStep = bottomStreets.empty() ? startSpace + step/2 : step;
        int curIndex = centerLineIndex;

        vector<Line> topLines , topLines2;

        Point startPoint1 = getNextPoint(lastPoint , centerLineIndex , centerL , newStep , topLines);
        Point startPoint2 = getNextPoint(lastPoint , curIndex , centerL , newStep + 5, topLines);


        if (startPoint1.getX() == INT_MAX || startPoint2.getX() == INT_MAX) break;
        topLines.clear();

        curIndex = centerLineIndex;

        lastPoint = getNextPoint(startPoint1 , centerLineIndex , centerL , step , topLines);
        Point lastPoint2 = getNextPoint(startPoint2 , curIndex , centerL , step-10 , topLines2);

        if (lastPoint.getX() == INT_MAX) break;

        Point next1UP = {startPoint1.getX() , startPoint1.getY() - height};

        Line nextLine (startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());

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


        Point next12UP = {startPoint2.getX() , startPoint2.getY() - height};

        nextLine =Line (startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());

        foundIntersection = false;
        for(auto &upLine : bottomLines)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next12UP = intersection;
                next12UP.setY(next12UP.getY() + 5);
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

        Point next22UP = {lastPoint2.getX() , lastPoint2.getY() - height};

        foundIntersection = false;
        nextLine = Line(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        for(auto &upLine : bottomLines)
        {
            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

            if (intersection.getX() != INT_MAX)
            {
                foundIntersection = true;
                next22UP = intersection;
                next22UP.setY(next22UP.getY() + 5);
                break;
            }
        }
        if (!foundIntersection)
        {
            continue;
        }

        vector<Line> homeLinesOuter , homeLinesInner;
        homeLinesOuter.insert(homeLinesOuter.end() , topLines.begin() , topLines.end());
        homeLinesOuter.emplace_back(startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());
        homeLinesOuter.emplace_back(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        homeLinesOuter.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        bottomStreets.push_back(homeLinesOuter);

        homeLinesInner.insert(homeLinesInner.end() , topLines2.begin() , topLines2.end());
        homeLinesInner.emplace_back(startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());
        homeLinesInner.emplace_back(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        homeLinesInner.emplace_back(next12UP.getX() , next12UP.getY() , next22UP.getX() , next22UP.getY());



        //EXTENSIONS And Border
        //vector<Line> extensions = drawExtensions(polygonLines , bottomLines , startPoint1 , lastPoint , next1UP , next2UP , step/2 , true, centerL);
        //vector<Line> homeBorder = drawHomeBorders( mainLand, homeLines , extensions , true);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
//        cityGrid.setRoadExtension(extensions);
//        cityGrid.setHomeBorder(homeBorder);

        cities.push_back(cityGrid);

    }
    return bottomStreets;
}

vector<Line>
DrawStreet::drawHomeBorders(Polygon1 &polygon1, vector<Line> &streetsLines,
                            vector<Line> &extensionsLine , bool isTop) {

    vector<Line> homeBorderSol;
    Point start = {streetsLines[streetsLines.size() -3 ].getX1() , streetsLines[streetsLines.size() -3 ].getY1()};
    Point last = {streetsLines[streetsLines.size() -2 ].getX1() , streetsLines[streetsLines.size() -2 ].getY1()};
    Point center = {extensionsLine[0 ].getX1() , extensionsLine[0 ].getY1()};

    Point startTOP = {streetsLines[streetsLines.size() -3 ].getX2() , streetsLines[streetsLines.size() -3 ].getY2()};
    Point lastTOP = {streetsLines[streetsLines.size() -2 ].getX2() , streetsLines[streetsLines.size() -2 ].getY2()};
    Point centerTOP = {extensionsLine[0 ].getX2() , extensionsLine[0 ].getY2()};

    Line startLine (start.getX() , start.getY() , startTOP.getX() , startTOP.getY() );
    Line endLine (lastTOP.getX() , lastTOP.getY() , last.getX() , last.getY() );
    Line topLine (startTOP.getX() , startTOP.getY() , lastTOP.getX() , lastTOP.getY() );
    Line centerLine (center.getX() , center.getY() , centerTOP.getX() , centerTOP.getY() );

    double minLength = min({startLine.getLength() , endLine.getLength() , centerLine.getLength()});

    double step = minLength / (int)(minLength/20);

    while (true)
    {
        Point nextPoint1 = PolygonHelper::getNextPoint(start , startTOP , step);
        Point nextPoint2 = PolygonHelper::getNextPoint(last , lastTOP , step);
        Point nextPoint3 = PolygonHelper::getNextPoint(center , centerTOP , step);

        if (nextPoint1 == startTOP || nextPoint2 == lastTOP || nextPoint3 == centerTOP)
        {
            break;
        }

        homeBorderSol.emplace_back(nextPoint1.getX() , nextPoint1.getY() , nextPoint3.getX() , nextPoint3.getY());
        homeBorderSol.emplace_back(nextPoint2.getX() , nextPoint2.getY() , nextPoint3.getX() , nextPoint3.getY());

        start = nextPoint1;
        last = nextPoint2;
        center = nextPoint3;
    }

    vector<Line> streetsOrder = {startLine , topLine , endLine};

    double height = 100000000000;

    int centerLineIndex = 0;
    vector<Line> outsideBorder;
    Point lastPoint = {streetsOrder[0].getX1() , streetsOrder[0].getY1() };

    double length = startLine.getLength() + endLine.getLength() + topLine.getLength();

    step = length / (int)(length/20);

    vector<Line >topLines = isTop ? PolygonHelper::getTopLines(polygon1 , 10) : PolygonHelper::getBottomLines(polygon1 , 10) ;
    vector<Line> polygonLines = polygon1.getLines();
    while(centerLineIndex < streetsOrder.size())
    {
        vector<Line> bottomLines;

        Point startPoint = getNextPoint(lastPoint , centerLineIndex , streetsOrder , step , bottomLines);

        if (startPoint.getX() == INT_MAX) break;
        bottomLines.clear();

        lastPoint = getNextPoint(startPoint , centerLineIndex , streetsOrder , step , bottomLines);

        if (lastPoint.getX() == INT_MAX) break;
        Line st = {lastPoint.getX() , lastPoint.getY() , streetsOrder[centerLineIndex].getX2() , streetsOrder[centerLineIndex].getY2()};
        if (centerLineIndex == 2 && st.getLength() < 20)break;

        int index1 = -1, index2 = -1;

        for (int i = 0; i < streetsOrder.size(); ++i)
        {

            const Line c = streetsOrder[i];
            double minX = min(c.getX1() , c.getX2());
            double maxX = max(c.getX1() , c.getX2());
            double minY = min(c.getY1() , c.getY2());
            double maxY = max(c.getY1() , c.getY2());

            if (startPoint.getX() >= minX && startPoint.getX() <= maxX &&
                    startPoint.getY() >= minY && startPoint.getY() <= maxY)
            {
                index1 = i;
            }
            if (lastPoint.getX() >= minX && lastPoint.getX() <= maxX &&
                    lastPoint.getY() >= minY && lastPoint.getY() <= maxY)
            {
                index2 = i;
            }
        }

        Point next1UP ;

        if (index1 == 0 )
        {
            next1UP = {startPoint.getX() - height , startPoint.getY()};
        }
        else if (index1 == 1)
        {
            if (isTop)next1UP = {startPoint.getX() , startPoint.getY() + height};
            else next1UP = {startPoint.getX() , startPoint.getY() - height};
        }
        else if (index1 == 2)
        {
            next1UP = {startPoint.getX() + height , startPoint.getY()};
        }

        Line nextLine (startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());

        bool foundIntersection = false;
        for(auto &upLine : extensionsLine)
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
            for(auto &upLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    foundIntersection = true;
                    next1UP = intersection;
                    break;
                }
            }
        }


        Point next2UP ;

        if (index2 == 0 )
        {
            next2UP = {lastPoint.getX() - height , lastPoint.getY()};
        }
        else if (index2 == 1)
        {
            if (isTop)next2UP = {lastPoint.getX() , lastPoint.getY() + height};
            else next2UP = {lastPoint.getX() , lastPoint.getY() - height};
        }
        else if (index2 == 2)
        {
            next2UP = {lastPoint.getX() + height , lastPoint.getY()};
        }

        foundIntersection = false;
        nextLine = Line(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        for(auto &upLine : extensionsLine)
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
            for(auto &upLine : polygonLines)
            {
                Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);

                if (intersection.getX() != INT_MAX)
                {
                    foundIntersection = true;
                    next2UP = intersection;
                    break;
                }
            }
        }

        vector<Line> homeLines;
        homeLines.insert(homeLines.end() , bottomLines.begin() , bottomLines.end());
        homeLines.emplace_back(startPoint.getX() , startPoint.getY() , next1UP.getX() , next1UP.getY());
//        homeLines.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());
        homeLines.emplace_back(next2UP.getX() , next2UP.getY() , lastPoint.getX() , lastPoint.getY());

        homeBorderSol.insert(homeBorderSol.end() , homeLines.begin() , homeLines.end());
    }

    return homeBorderSol;
}

vector<Line> DrawStreet::drawExtensions(const vector<Line> &polygonLines ,const vector<Line> &topLines , const Point &start, const Point &end, const Point &startUp, const Point &endUp,double step , bool isTop ,const vector<Line> &centerL)
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

    centerTop = Point ((startUp.getX()+endUp.getX())/2 , (startUp.getY()+endUp.getY())/2);

    Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
    extensions.push_back(centerExtension);

    int lineIndex1 = 0 , lineIndex2 = 0;

    for (int i = 0; i < centerL.size(); ++i) {
        const Line& c = centerL[i];

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
    Point nextEnd = getNextPoint(end , lineIndex2 , centerL , step , tt);
    Point prevStart = getPrevPoint(start , lineIndex1 , centerL , step , tt);


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

const vector<CityGrid> &DrawStreet::getCities() const {
    return cities;
}
















