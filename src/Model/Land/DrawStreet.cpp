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

    Polygon1 innerPolygon = PolygonHelper::getScalingPolygon(polygon1 , -20);

    vector<Line> spacingLines = innerPolygon.getLines();

//    vector<Line> topPoints = PolygonHelper::getTopLines(polygon1 , 30);
//    vector<Line> bottomPoints = PolygonHelper::getBottomLines(polygon1 , 30);


    int divisions = 0 , index = 0 , divisionsB = 1;
    vector<Line> bo;
    Point start = getNextPoint({centerLinesTop[index].getX1() , centerLinesTop[index].getY1()} , index , centerLinesTop , startSpace , bo);

    while (true)
    {
        vector<Line> ll;
        Point next = getNextPoint(start , index , centerLinesTop , step1/2 , ll);
        if (next.getX() == INT_MAX) break;

        Line nextLine(next.getX() , next.getY() , centerLinesTop[index].getX2() , centerLinesTop[index].getY2());


        if (index == centerLinesTop.size()-1 && nextLine.getLength() <= 20) break;
        divisions++;
        start = next;
    }

    divisions/=4;
    index = 0;

    bo.clear();
    start = getNextPoint({centerLinesBottom[index].getX1() , centerLinesBottom[index].getY1()} , index , centerLinesTop , startSpace + step1/2, bo);

    while (true)
    {
        vector<Line> ll;
        Point next = getNextPoint(start , index , centerLinesBottom , step1/2 , ll);
        if (next.getX() == INT_MAX) break;

        Line nextLine(next.getX() , next.getY() , centerLinesBottom[index].getX2() , centerLinesBottom[index].getY2());


        if (index == centerLinesBottom.size()-1 && nextLine.getLength() <= 20) break;

        divisionsB++;
        start = next;
    }

    divisionsB/=4;

//    cout<<"divisions = "<<divisions <<"\n";
//    cout<<"divisionsB = "<<divisionsB <<"\n";

    divisions = min(divisions , divisionsB);

    double lengthC = 0;
    for(auto &line : centerLinesTop)
    {
        lengthC += line.getLength();
    }

    lengthC-=40;

    step1 = ((lengthC) / (int)(divisions*4)) * 2;
    cout<<"Length = "<<lengthC<<" -- New Step = "<<step1<<"\n";

    lengthC = 0;
    for(auto &line : centerLinesBottom)
    {
        lengthC += line.getLength();
    }
    lengthC-=40;

    step1 = min(step1 , ((lengthC) / (int)(divisions*4)) * 2);
    cout<<"Length = "<<lengthC<<" --New Step = "<<step1<<"\n";


    vector<Line> polygonLines = polygon1.getLines();

    vector<vector<Line>> topStreets = drawTopStreets(polygonLines ,centerLinesTop ,  spacingLines , step1 , divisions);
    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines ,centerLinesBottom, spacingLines , step1 , divisions);


//    streets.push_back(bottomPoints);


    vector<Line> centerTop = SplitCenterLines(startSpace , step1 , divisions ,centerLinesTop);
    //Bottoms
    vector<Line> centerBottoms = SplitCenterLines(startSpace + step1/2 , step1 , divisions , centerLinesBottom);
    centerLines.clear();
    centerLines = centerTop;
    centerLines.insert(centerLines.end() , centerBottoms.begin() , centerBottoms.end());
    centerLines.insert(centerLines.end() , spacingLines.begin() , spacingLines.end());

}

vector<Line> DrawStreet::SplitCenterLines(double startSpace,double step1 , int divisions, const vector<Line> &centerLines2) {
    int index = 0;
    Point stP = {centerLines2[index].getX1() , centerLines2[index].getY1()};
    vector<Line> centerTop;
    for (int i = 0; i < divisions; ++i)
    {
        vector<Line> bottoms;
        Point next = getNextPoint(stP , index , centerLines2 , startSpace , bottoms);
        startSpace = step1;

        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());
        bottoms.clear();

        stP = next;

        int index2 = index;
        next = getNextPoint(next , index2 , centerLines2 , 5 , bottoms);
        bottoms.clear();

        Point next2 = getNextPoint(next , index2 , centerLines2 , startSpace - 10 , bottoms);

        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());

        stP = getNextPoint(stP , index , centerLines2 , startSpace , bottoms);
    }
    Line l (stP.getX() , stP.getY() , centerLines2.back().getX2() , centerLines2.back().getY2());

    vector<Line> bottoms;
    stP = getNextPoint(stP , index , centerLines2 , l.getLength() + 100, bottoms);

    centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());

    return centerTop;
}



vector<vector<Line>> DrawStreet::drawTopStreets(const vector<Line> &polygonLines,const vector<Line> &centerL , const vector<Line> &topLine, double step ,int divisions)
{
    vector<Line>topLinesC = topLine;
    topLinesC.insert(topLinesC.end() , polygonLines.begin() , polygonLines.end());
    vector<vector<Line>> topStreets;
    double height = 100000000000;

    Line rightLine , lastLine;

    Point centerFirst = {centerL[0].getX1() , centerL[0].getY1()};
    Point centerLast = {centerL.back().getX2() , centerL.back().getY2()};

    for (auto &polLine : polygonLines)
    {
        double minX = min(polLine.getX1() , polLine.getX2());
        double maxX = max(polLine.getX1() , polLine.getX2());
        double minY = min(polLine.getY1() , polLine.getY2());
        double maxY = max(polLine.getY1() , polLine.getY2());

        if (centerFirst.getX() >= minX && centerFirst.getX() <= maxX &&
                centerFirst.getY() >= minY && centerFirst.getY() <= maxY)
        {
            rightLine = polLine;
        }

        if (centerLast.getX() >= minX && centerLast.getX() <= maxX &&
                centerLast.getY() >= minY && centerLast.getY() <= maxY)
        {
            lastLine = polLine;
        }
    }

    int centerLineIndex = 0;
    Point lastPoint = {centerL[0].getX1() , centerL[0].getY1() };
    for (int m = 0; m < divisions; ++m)
    {
        vector<Line> bottomLines , bottomLines2;

        double newStep = !m ? startSpace : step;
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

        if (!m)
        {
            next1UP = PolygonHelper::getOtherLinePoint(startPoint1 , PolygonHelper::getSlope(rightLine) , startPoint1.getY() + height);
        }

        Line nextLine (startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());

        bool foundIntersection = false;
        for(auto &upLine : topLinesC)
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

        if (!m)
        {
            next12UP = PolygonHelper::getOtherLinePoint(startPoint2 , PolygonHelper::getSlope(rightLine) , startPoint2.getY() + height);
        }

        nextLine =Line (startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());

        foundIntersection = false;
        for(auto &upLine : topLinesC)
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

        if (m == divisions - 1)
        {
            next2UP = PolygonHelper::getOtherLinePoint(lastPoint , PolygonHelper::getSlope(lastLine) , lastPoint.getY() + height);
        }

        foundIntersection = false;
        nextLine = Line(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        for(auto &upLine : topLinesC)
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

        if (m == divisions - 1)
        {
            next22UP = PolygonHelper::getOtherLinePoint(lastPoint2 , PolygonHelper::getSlope(lastLine) , lastPoint2.getY() + height);
        }

        foundIntersection = false;
        nextLine = Line(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        for(auto &upLine : topLinesC)
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
        homeLinesOuter.emplace_back(next2UP.getX() , next2UP.getY() , lastPoint.getX() , lastPoint.getY());        homeLinesOuter.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        topStreets.push_back(homeLinesOuter);
        homeLinesInner.insert(homeLinesInner.end() , bottomLines2.begin() , bottomLines2.end());
        homeLinesInner.emplace_back(startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());
        homeLinesInner.emplace_back(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        homeLinesInner.emplace_back(next12UP.getX() , next12UP.getY() , next22UP.getX() , next22UP.getY());


        //EXTENSIONS And Border
        vector<Line> extensions = drawExtensions(polygonLines , bottomLines2 , startPoint2 , lastPoint2 , next12UP , next22UP , step/2 +5, true, centerL);
        vector<Line> homeBorder = drawHomeBorders( mainLand, homeLinesOuter , homeLinesInner , extensions , true);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
        cityGrid.setRoadExtension(extensions);
        cityGrid.setHomeBorder(homeBorder);

        cities.push_back(cityGrid);
    }
    return topStreets;
}

vector<vector<Line>> DrawStreet::drawBottomStreets(const vector<Line> &polygonLines,const vector<Line> &centerL , const vector<Line> &bottomLines, double step ,int divisions)
{
    vector<Line> bottomLinesC = bottomLines;
    bottomLinesC.insert(bottomLinesC.end() , polygonLines.begin() , polygonLines.end());
    vector<vector<Line>> bottomStreets;
    double height = 100000000000;

    Line rightLine , lastLine;

    Point centerFirst = {centerL[0].getX1() , centerL[0].getY1()};
    Point centerLast = {centerL.back().getX2() , centerL.back().getY2()};

    for (auto &polLine : polygonLines)
    {
        double minX = min(polLine.getX1() , polLine.getX2());
        double maxX = max(polLine.getX1() , polLine.getX2());
        double minY = min(polLine.getY1() , polLine.getY2());
        double maxY = max(polLine.getY1() , polLine.getY2());

        if (centerFirst.getX() >= minX && centerFirst.getX() <= maxX &&
            centerFirst.getY() >= minY && centerFirst.getY() <= maxY)
        {
            rightLine = polLine;
        }

        if (centerLast.getX() >= minX && centerLast.getX() <= maxX &&
            centerLast.getY() >= minY && centerLast.getY() <= maxY)
        {
            lastLine = polLine;
        }
    }

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

        if (!m)
        {
            next1UP = PolygonHelper::getOtherLinePoint(startPoint1 , PolygonHelper::getSlope(rightLine) , startPoint1.getY() - height);
        }

        Line nextLine (startPoint1.getX() , startPoint1.getY() , next1UP.getX() , next1UP.getY());

        bool foundIntersection = false;
        for(auto &upLine : bottomLinesC)
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

        if (!m)
        {
            next12UP = PolygonHelper::getOtherLinePoint(startPoint2 , PolygonHelper::getSlope(rightLine) , startPoint2.getY() - height);
        }

        nextLine =Line (startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());

        foundIntersection = false;
        for(auto &upLine : bottomLinesC)
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

        if (m == divisions - 1)
        {
            next2UP = PolygonHelper::getOtherLinePoint(lastPoint , PolygonHelper::getSlope(lastLine) , lastPoint.getY() - height);
        }

        foundIntersection = false;
        nextLine = Line(lastPoint.getX() , lastPoint.getY() , next2UP.getX() , next2UP.getY());
        for(auto &upLine : bottomLinesC)
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

        if (m == divisions - 1)
        {
            next22UP = PolygonHelper::getOtherLinePoint(lastPoint2 , PolygonHelper::getSlope(lastLine) , lastPoint2.getY() - height);
        }

        foundIntersection = false;
        nextLine = Line(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        for(auto &upLine : bottomLinesC)
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
        homeLinesOuter.emplace_back(next2UP.getX() , next2UP.getY() , lastPoint.getX() , lastPoint.getY());
        homeLinesOuter.emplace_back(next1UP.getX() , next1UP.getY() , next2UP.getX() , next2UP.getY());

        bottomStreets.push_back(homeLinesOuter);

        homeLinesInner.insert(homeLinesInner.end() , topLines2.begin() , topLines2.end());
        homeLinesInner.emplace_back(startPoint2.getX() , startPoint2.getY() , next12UP.getX() , next12UP.getY());
        homeLinesInner.emplace_back(lastPoint2.getX() , lastPoint2.getY() , next22UP.getX() , next22UP.getY());
        homeLinesInner.emplace_back(next12UP.getX() , next12UP.getY() , next22UP.getX() , next22UP.getY());



        //EXTENSIONS And Border
        vector<Line> extensions = drawExtensions(polygonLines , topLines2 , startPoint2 , lastPoint2 , next12UP , next22UP , step/2 +5, false, centerL);
        vector<Line> homeBorder = drawHomeBorders( mainLand, homeLinesOuter , homeLinesInner , extensions , false);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
        cityGrid.setRoadExtension(extensions);
        cityGrid.setHomeBorder(homeBorder);

        cities.push_back(cityGrid);

    }
    return bottomStreets;
}

vector<Line>
DrawStreet::drawHomeBorders(Polygon1 &polygon1, vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner ,
                            vector<Line> &extensionsLine , bool isTop) {

    vector<Line> homeBorderSol;
    Point start = {streetsLinesInner[streetsLinesInner.size() -3 ].getX1() , streetsLinesInner[streetsLinesInner.size() -3 ].getY1()};
    Point last = {streetsLinesInner[streetsLinesInner.size() -2 ].getX1() , streetsLinesInner[streetsLinesInner.size() -2 ].getY1()};
    Point center = {extensionsLine[0 ].getX1() , extensionsLine[0 ].getY1()};

    Point startTOP = {streetsLinesInner[streetsLinesInner.size() -3 ].getX2() , streetsLinesInner[streetsLinesInner.size() -3 ].getY2()};
    Point lastTOP = {streetsLinesInner[streetsLinesInner.size() -2 ].getX2() , streetsLinesInner[streetsLinesInner.size() -2 ].getY2()};
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


    start = {streetsLinesOuter[streetsLinesOuter.size() -3 ].getX1() , streetsLinesOuter[streetsLinesInner.size() -3 ].getY1()};
    last = {streetsLinesOuter[streetsLinesOuter.size() -2 ].getX1() , streetsLinesOuter[streetsLinesInner.size() -2 ].getY1()};

    startTOP = {streetsLinesOuter[streetsLinesOuter.size() -3 ].getX2() , streetsLinesOuter[streetsLinesOuter.size() -3 ].getY2()};
    lastTOP = {streetsLinesOuter[streetsLinesOuter.size() -2 ].getX2() , streetsLinesOuter[streetsLinesOuter.size() -2 ].getY2()};

    startLine  = streetsLinesOuter[streetsLinesOuter.size() -3];
    endLine = streetsLinesOuter[streetsLinesOuter.size() -2];
    topLine = streetsLinesOuter[streetsLinesOuter.size() -1];


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

        lastPoint = getNextPoint(lastPoint , centerLineIndex , streetsOrder , step , bottomLines);

        if (lastPoint.getX() == INT_MAX) break;


        Line st = {lastPoint.getX() , lastPoint.getY() , streetsOrder[centerLineIndex].getX2() , streetsOrder[centerLineIndex].getY2()};
        if (centerLineIndex == 2 && st.getLength() < 20)break;

        int index1 = -1;

        for (int i = 0; i < streetsOrder.size(); ++i)
        {
            Line line = streetsOrder[i];
            double minX = min(line.getX1() , line.getX2());
            double maxX = max(line.getX1() , line.getX2());
            double minY = min(line.getY1() , line.getY2());
            double maxY = max(line.getY1() , line.getY2());

            if (lastPoint.getX() >= minX && lastPoint.getX() <= maxX &&
                    lastPoint.getY() >= minY && lastPoint.getY() <= maxY)
            {
                index1 = i;
                break;
            }
        }


        Point next1UP ;

        bool foundIntersection = false;


        if (index1 == 0 )
        {
            next1UP = {lastPoint.getX() - height , lastPoint.getY()};
        }
        else if (index1 == 1)
        {
            if (isTop)next1UP = {lastPoint.getX() , lastPoint.getY() + height};
            else next1UP = {lastPoint.getX() , lastPoint.getY() - height};
        }
        else if (index1 == 2)
        {
            next1UP = {lastPoint.getX() + height , lastPoint.getY()};
        }

        foundIntersection = false;
        Line nextLine (lastPoint.getX() , lastPoint.getY() , next1UP.getX() , next1UP.getY());
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

        vector<Line> homeLines;
        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next1UP.getX() , next1UP.getY());
        homeBorderSol.insert(homeBorderSol.end() , homeLines.begin() , homeLines.end());
    }

    return homeBorderSol;
}

vector<Line> DrawStreet::drawExtensions(const vector<Line> &polygonLines ,const vector<Line> &topLines , const Point &start, const Point &end, const Point &startUp, const Point &endUp,double step , bool isTop ,const vector<Line> &centerL)
{
    vector<Line> extensions;
    Point centerBottom(100,100) , centerTop;
    double reqLength = 0;

    for(auto &bLine : topLines)reqLength +=bLine.getLength();

    reqLength/=2;

//    cout<<"reqLength = "<<reqLength<<"\n";

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


















