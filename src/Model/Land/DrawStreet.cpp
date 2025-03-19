//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

void DrawStreet::drawStreets(Polygon1 &polygon1)
{
    double step1 = step ;
    mainLand = polygon1;
    vector<Line> polygonLines = polygon1.getLines();
    centerLines = buildCenterLines(polygon1 , 10);

    vector<Line> centerLinesTop , centerLinesBottom;

    for (int i = 0; i < centerLines.size(); ++i)
    {
        if (i<centerLines.size()/2)centerLinesTop.push_back(centerLines[i]);
        else centerLinesBottom.push_back(centerLines[i]);
    }

    Polygon1 innerPolygon = PolygonHelper::getScalingPolygon(polygon1 , -20);

    if (innerPolygon.getArea() > polygon1.getArea())
    {
        innerPolygon = PolygonHelper::getScalingPolygon(polygon1 , 20);
    }

    vector<Line> spacingLines = innerPolygon.getLines();
    vector<Line> testLines;
    vector<Line> lines = mainLand.getLines();


    int divisions = ((int)((centerLinesTop[0].getLength()/20) - 1) / 4);
    int divisionsB = ((int)((centerLinesBottom[0].getLength()/20)-1) / 4);


    cout<<"divisions = "<<divisions <<"\n";
    cout<<"divisionsB = "<<divisionsB <<"\n";

    divisions = min(divisions , divisionsB);

    double lengthC = 0;
    for(auto &line : centerLinesTop)
    {
        lengthC += line.getLength();
    }

    step1 = ((lengthC / (((int)(divisions*4)) + 1))) * 2;

    cout<<"Id = "<<polygon1.getId()<<"  divisions = "<<divisions << " -- "<<divisionsB<<"  Length = "<<lengthC<<" -- New Step = "<<step1<<"\n";

    lengthC = 0;
    for(auto &line : centerLinesBottom)
    {
        lengthC += line.getLength();
    }

    step1 = min(step1 , ((lengthC / (((int)(divisions*4)) + 1))) * 2);
    cout<<"Length = "<<lengthC<<" --New Step = "<<step1<<"\n";

    startSpace = step1 / 2;
    drawSide1Streets(polygonLines ,centerLinesTop ,  spacingLines , step1 , divisions);
    startSpace = step1;
    drawSide1Streets(polygonLines ,centerLinesBottom ,  spacingLines , step1 , divisions);
//    vector<vector<Line>> bottomStreets = drawBottomStreets(polygonLines ,centerLinesBottom, spacingLines , step1 , divisions);



//    vector<Line> centerTop = SplitCenterLines(startSpace , step1 , divisions ,centerLinesTop);
//    //Bottoms
//    vector<Line> centerBottoms = SplitCenterLines(startSpace + step1/2 , step1 , divisions , centerLinesBottom);
//    centerLines.clear();
//    centerLines = centerTop;
//    centerLines.insert(centerLines.end() , centerBottoms.begin() , centerBottoms.end());
    centerLines.insert(centerLines.end() , spacingLines.begin() , spacingLines.end());

}

vector<Line> DrawStreet::SplitCenterLines(double startSpace,double step1 , int divisions, const vector<Line> &centerLines2) {
    int index = 0;
    Point stP = {centerLines2[index].getX1() , centerLines2[index].getY1()};
    vector<Line> centerTop;
//    for (int i = 0; i < divisions; ++i)
//    {
//        Point next = getNextPoint(stP , index , centerLines2 , startSpace );
//        startSpace = step1;
//
//        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());
//        bottoms.clear();
//
//        stP = next;
//
//        int index2 = index;
//        next = getNextPoint(next , index2 , centerLines2 , 5 , bottoms);
//        bottoms.clear();
//
//        Point next2 = getNextPoint(next , index2 , centerLines2 , startSpace - 10 , bottoms);
//
//        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());
//
//        stP = getNextPoint(stP , index , centerLines2 , startSpace , bottoms);
//    }
//    Line l (stP.getX() , stP.getY() , centerLines2.back().getX2() , centerLines2.back().getY2());
//
//    vector<Line> bottoms;
//    stP = getNextPoint(stP , index , centerLines2 , l.getLength() + 100, bottoms);
//
//    centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());


    return centerTop;
}

void DrawStreet::drawSide1Streets(const vector<Line> &polygonLines,const vector<Line> &centerL , const vector<Line> &topLine, double step ,int divisions)
{
    Line otherCenter = centerL[0]== centerLines[0] ? centerLines.back() : centerLines[0];


    vector<vector<Line>> side1Streets;

    Point centerFirst = {centerL[0].getX1() , centerL[0].getY1()};
    Point centerLast = {centerL[0].getX2() , centerL[0].getY2()};

    Line side1Line = PolygonHelper::getLineForPoint(polygonLines , centerFirst);
    Line side2Line = PolygonHelper::getLineForPoint(polygonLines , centerLast);

    int centerLineIndex = 0;
    Point lastPoint = centerFirst;

    int opp = 1;


    for (int m = 0; m < divisions; ++m)
    {
        double newStep = !m ? startSpace : step;

        Point last = lastPoint;

        Point startPoint1 = getNextPoint(lastPoint , centerLineIndex , centerL , newStep );

        if (startPoint1.getX() == INT_MAX ) break;

        Line testLine (startPoint1.getX(), startPoint1.getY() , lastPoint.getX() , lastPoint.getY());


        lastPoint = getNextPoint(startPoint1 , centerLineIndex , centerL , step );

        if (lastPoint.getX() == INT_MAX) break;

        const Line& centerLine = centerL[centerLineIndex];
        double slope1 = m ? centerLine.getSlope() : side1Line.getSlope();
        double slope2 = (m < divisions - 1 )? centerLine.getSlope() : side2Line.getSlope();

        if (slope1 == centerLine.getSlope() && slope1 != 0)
        {
            slope1 = -1 / slope1;
        }

        if (slope2 == centerLine.getSlope() && slope2 != 0)
        {
            slope2 = -1 / slope2;
        }

        Point next1UP ;
        Point next2UP ;
        if (slope1 == 0 && centerLine.getY1() == centerLine.getY2())
        {
           slope1 = -1;
        }

        if (slope2 == 0 && centerLine.getY1() == centerLine.getY2())
        {
            slope2 = -1;
        }


        vector<Point> cPoints = {startPoint1 , lastPoint};
        vector<double> slopes = {slope1 , slope2};
        vector<Line> streetsBorder;


        for (int i = 0; i < cPoints.size(); ++i)
        {
            Point cur = cPoints[i];
            Line cuttingLine (cur.getX() , cur.getY() , cur.getX() , cur.getY());

            if (slopes[i] == 0)
            {
                cuttingLine.setY2(cuttingLine.getY1() - 100000000*opp);
            }
            else if (slopes[i] == -1)
            {
                cuttingLine.setX2(cuttingLine.getX1() - 100000000*opp);
            }
            else
            {
                Point second = PolygonHelper::getSecondLinePoint(cur , slopes[i] , 200000*opp);
                cuttingLine.setX2(second.getX());
                cuttingLine.setY2(second.getY());
            }

            Point intersectionPoint1 = PolygonHelper::getIntersectionPoint(cuttingLine , otherCenter);

            if ( intersectionPoint1.getX() != INT_MAX)
            {
                opp = opp==1?-1:1;
                i--;
                continue;
            }

            for(auto &line2 : topLine)
            {
                Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line2);
                if (intersectionPoint.getX() != INT_MAX)
                {
                    cuttingLine.setX2(intersectionPoint.getX());
                    cuttingLine.setY2(intersectionPoint.getY());
                    break;
                }
            }

            streetsBorder.push_back(cuttingLine);
        }

        Point testIntersect = PolygonHelper::getIntersectionPoint(streetsBorder[0] , streetsBorder[1]);

        if (testIntersect.getX() != INT_MAX)
        {
            next1UP = testIntersect;
            next2UP = testIntersect;
        }
        else
        {
            next1UP = {streetsBorder[0].getX2() , streetsBorder[0].getY2()};
            next2UP = {streetsBorder[1].getX2() , streetsBorder[1].getY2()};
        }

        // next1UP , next2UP
        Polygon1 outerStreet({startPoint1 ,next1UP , next2UP, lastPoint});

        vector<Line> homeLinesOuter = outerStreet.getLines();

        vector<Point> innerPoint;
        if (next1UP == next2UP)
        {
            innerPoint = {startPoint1 , next1UP , lastPoint};
        }
        else{
            innerPoint = {startPoint1 , next1UP, next2UP , lastPoint};
        }

        Polygon1 innerStreet (innerPoint) , innerCopy(innerPoint);
        innerStreet = PolygonHelper::getScalingPolygon(innerCopy , -5);

        if (innerStreet.getArea() > outerStreet.getArea())
        {
            innerStreet = PolygonHelper::getScalingPolygon(innerCopy , 5);
        }


        vector<Point> innerPoints = innerStreet.getPoints();

        if (next1UP == next2UP)
        {
            innerPoints = {innerPoints[2] , innerPoints[0] , innerPoints[0] , innerPoints[1]};
        }
        else{
            innerPoints = {innerPoints[3] , innerPoints[0] , innerPoints[1] , innerPoints[2]};
        }

        double dx = innerPoints[1].getX() - innerPoints[0].getX();
        double dy = innerPoints[1].getY() - innerPoints[0].getY();

        innerPoints[0].setX(innerPoints[1].getX() - dx * 10);
        innerPoints[0].setY(innerPoints[1].getY() - dy * 10);

        dx = innerPoints[2].getX() - innerPoints[3].getX();
        dy = innerPoints[2].getY() - innerPoints[3].getY();

        innerPoints[3].setX(innerPoints[3].getX() + dx * -10);
        innerPoints[3].setY(innerPoints[3].getY() + dy * -10);

        Line f1 (innerPoints[2].getX() , innerPoints[2].getY() , innerPoints[3].getX() , innerPoints[3].getY());
        Line f2 (innerPoints[1].getX() , innerPoints[1].getY() , innerPoints[0].getX() , innerPoints[0].getY());

        Point inter = PolygonHelper::getIntersectionPoint(f1 , centerLine);
        Point inter2 = PolygonHelper::getIntersectionPoint(f2 , centerLine);

        if (inter.getX() != INT_MAX)
        {
            innerPoints[3] = inter;
        }

        if (inter2.getX() != INT_MAX)
        {
            innerPoints[0] = inter2;
        }

        innerStreet = Polygon1(innerPoints);

        vector<Line> homeLinesInner = innerStreet.getLines();
        Line innerBottomLine = {innerPoints[0].getX() , innerPoints[0].getY() , innerPoints[3].getX() , innerPoints[3].getY()};

        //EXTENSIONS And Border
        vector<Line> extensions = drawExtensions(polygonLines , innerBottomLine , innerPoints[0] , innerPoints[3] , innerPoints[1] , innerPoints[2] , step/2 +5, centerL);
        vector<Line> homeBorder = drawHomeBorders( mainLand, homeLinesOuter , homeLinesInner , extensions);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
        cityGrid.setRoadExtension(extensions);
        cityGrid.setHomeBorder(homeBorder);

        cities.push_back(cityGrid);
    }
}

vector<Line>
DrawStreet::drawHomeBorders(Polygon1 &polygon1, vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner ,
                            vector<Line> &extensionsLine ) {

    vector<Line> homeBorderSol;
    Point start = {streetsLinesInner[0 ].getX1() , streetsLinesInner[0 ].getY1()};
    Point last = {streetsLinesInner.back().getX1() , streetsLinesInner.back().getY1()};
    Point center = {extensionsLine[0 ].getX1() , extensionsLine[0 ].getY1()};

    Point startTOP  = {streetsLinesInner[1 ].getX2() , streetsLinesInner[1 ].getY2()};
    Point lastTOP   = {streetsLinesInner[2 ].getX2() , streetsLinesInner[2 ].getY2()};
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


//    start = {streetsLinesOuter[streetsLinesOuter.size() -3 ].getX1() , streetsLinesOuter[streetsLinesInner.size() -3 ].getY1()};
//    last = {streetsLinesOuter[streetsLinesOuter.size() -2 ].getX1() , streetsLinesOuter[streetsLinesInner.size() -2 ].getY1()};
//
//    startTOP = {streetsLinesOuter[streetsLinesOuter.size() -3 ].getX2() , streetsLinesOuter[streetsLinesOuter.size() -3 ].getY2()};
//    lastTOP = {streetsLinesOuter[streetsLinesOuter.size() -2 ].getX2() , streetsLinesOuter[streetsLinesOuter.size() -2 ].getY2()};
//
//    startLine  = streetsLinesOuter[streetsLinesOuter.size() -3];
//    endLine = streetsLinesOuter[streetsLinesOuter.size() -2];
//    topLine = streetsLinesOuter[streetsLinesOuter.size() -1];
//
//
//    vector<Line> streetsOrder = {startLine , topLine , endLine};
//
//    double height = 100000000000;
//
//    int centerLineIndex = 0;
//    vector<Line> outsideBorder;
//    Point lastPoint = {streetsOrder[0].getX1() , streetsOrder[0].getY1() };
//
//    double length = startLine.getLength() + endLine.getLength() + topLine.getLength();
//
//    step = length / (int)(length/20);
//
//    vector<Line >topLines = isTop ? PolygonHelper::getTopLines(polygon1 , 10) : PolygonHelper::getBottomLines(polygon1 , 10) ;
//    vector<Line> polygonLines = polygon1.getLines();
//    while(centerLineIndex < streetsOrder.size())
//    {
//        vector<Line> bottomLines;
//
//        lastPoint = getNextPoint(lastPoint , centerLineIndex , streetsOrder , step , bottomLines);
//
//        if (lastPoint.getX() == INT_MAX) break;
//
//
//        Line st = {lastPoint.getX() , lastPoint.getY() , streetsOrder[centerLineIndex].getX2() , streetsOrder[centerLineIndex].getY2()};
//        if (centerLineIndex == 2 && st.getLength() < 20)break;
//
//        int index1 = -1;
//
//        for (int i = 0; i < streetsOrder.size(); ++i)
//        {
//            Line line = streetsOrder[i];
//            double minX = min(line.getX1() , line.getX2());
//            double maxX = max(line.getX1() , line.getX2());
//            double minY = min(line.getY1() , line.getY2());
//            double maxY = max(line.getY1() , line.getY2());
//
//            if (lastPoint.getX() >= minX && lastPoint.getX() <= maxX &&
//                    lastPoint.getY() >= minY && lastPoint.getY() <= maxY)
//            {
//                index1 = i;
//                break;
//            }
//        }
//
//
//        Point next1UP ;
//
//        bool foundIntersection = false;
//
//
//        if (index1 == 0 )
//        {
//            next1UP = {lastPoint.getX() - height , lastPoint.getY()};
//        }
//        else if (index1 == 1)
//        {
//            if (isTop)next1UP = {lastPoint.getX() , lastPoint.getY() + height};
//            else next1UP = {lastPoint.getX() , lastPoint.getY() - height};
//        }
//        else if (index1 == 2)
//        {
//            next1UP = {lastPoint.getX() + height , lastPoint.getY()};
//        }
//
//        foundIntersection = false;
//        Line nextLine (lastPoint.getX() , lastPoint.getY() , next1UP.getX() , next1UP.getY());
//        for(auto &upLine : extensionsLine)
//        {
//            Point intersection = PolygonHelper::getIntersectionPoint(upLine , nextLine);
//
//            if (intersection.getX() != INT_MAX)
//            {
//                foundIntersection = true;
//                next1UP = intersection;
//                break;
//            }
//        }
//        if (!foundIntersection)
//        {
//            for(auto &upLine : polygonLines)
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
//        }
//
//        vector<Line> homeLines;
//        homeLines.emplace_back(lastPoint.getX() , lastPoint.getY() , next1UP.getX() , next1UP.getY());
//        homeBorderSol.insert(homeBorderSol.end() , homeLines.begin() , homeLines.end());
//    }

    return homeBorderSol;
}

vector<Line> DrawStreet::drawExtensions(const vector<Line> &polygonLines ,const Line &bottomLine , const Point &start, const Point &end, const Point &startUp, const Point &endUp,double step ,const vector<Line> &centerL)
{
    vector<Line> extensions;
    Point centerBottom((bottomLine.getX2() + bottomLine.getX1()) / 2,(bottomLine.getY2() + bottomLine.getY1()) / 2) , centerTop;
    double reqLength = bottomLine.getLength() / 2;

    centerTop = Point ((startUp.getX()+endUp.getX())/2 , (startUp.getY()+endUp.getY())/2);

    Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
    extensions.push_back(centerExtension);

    return extensions;

//    int lineIndex1 = 0 , lineIndex2 = 0;
//
//    for (int i = 0; i < centerL.size(); ++i) {
//        const Line& c = centerL[i];
//
//        double minX = min(c.getX1() , c.getX2());
//        double maxX = max(c.getX1() , c.getX2());
//        double minY = min(c.getY1() , c.getY2());
//        double maxY = max(c.getY1() , c.getY2());
//
//        if (start.getX() >= minX && start.getX() <= maxX &&
//        start.getY() >= minY && start.getY() <= maxY)
//        {
//            lineIndex1 = i;
//        }
//        if (end.getX() >= minX && end.getX() <= maxX &&
//                end.getY() >= minY && end.getY() <= maxY)
//        {
//            lineIndex2 = i;
//        }
//    }
//    Point nextEnd = getNextPoint(end , lineIndex2 , centerL , step );
//    Point prevStart = getPrevPoint(start , lineIndex1 , centerL , step );
//
//
//    Line rightExtension (nextEnd.getX() , nextEnd.getY() , nextEnd.getX() , nextEnd.getY() + (1000000000 * (isTop? 1 :-1)));
//    Line leftExtension (prevStart.getX() , prevStart.getY() , prevStart.getX() , prevStart.getY() + (1000000000 * (isTop? 1 :-1)));
//
//    for(auto &pLine : polygonLines)
//    {
//        Point intersection = PolygonHelper::getIntersectionPoint(pLine , rightExtension);
//
//        if (intersection.getX() != INT_MAX)
//        {
//            rightExtension.setY2(intersection.getY());
//            break;
//        }
//    }
//    extensions.push_back(rightExtension);
//
//    for(auto &pLine : polygonLines)
//    {
//        Point intersection = PolygonHelper::getIntersectionPoint(pLine , leftExtension);
//
//        if (intersection.getX() != INT_MAX)
//        {
//            leftExtension.setY2(intersection.getY());
//            break;
//        }
//    }
//    extensions.push_back(leftExtension);

    return extensions;
}

Point DrawStreet::getNextPoint(const Point &start, int &lineIndex, const vector<Line> &lines, double step )
{
    if (lineIndex == lines.size())
    {
        return {INT_MAX, INT_MAX};
    }

    Point endPoint , startPoint = start;
    while (true)
    {
        const Line& line = lines[lineIndex];
        Point destination = {line.getX2() , line.getY2()};
        Line partialLine {startPoint.getX() , startPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length < step)
        {
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
            break;
        }
    }
    return endPoint;
}

Point DrawStreet::getPrevPoint(const Point &end, int &lineIndex, const vector<Line> &lines, double step )
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

vector<Line> DrawStreet::buildCenterLines(Polygon1 &polygon, double centerLineHeight)
{
    vector<Line> cLines;

    vector<Line> centerLine = polygon.computeCentroidPerpendiculars();

    vector<Point> points = polygon.getPoints();
    vector<Line> lines = polygon.getLines();
    int n = (int)points.size();
    vector<Point> centerPoints ;
    vector<Point> centerPointsTOP ;
    vector<Point> centerPointsBottom ;

    vector<pair<double , Line>> sortCenterLines;

    for(auto &line : centerLine)
    {
        Point two(line.getX2() , line.getY2());

        for(auto &line2 : lines)
        {
            double x1 = min(line2.getX1() , line2.getX2()), x2 = max(line2.getX1() , line2.getX2());
            double y1 = min(line2.getY1() , line2.getY2()), y2 = max(line2.getY1() , line2.getY2());

            if (PolygonHelper::isPointOnLine(two , line2))continue;

            Point intersectionPoint = PolygonHelper::getIntersectionPoint(line , line2);
            if (intersectionPoint.getX() != INT_MAX)
            {
                line.setX1(intersectionPoint.getX());
                line.setY1(intersectionPoint.getY());
                break;
            }
        }
    }

    for(auto &line : centerLine)
    {
        double minLength = 1000000000000;
        Point one(line.getX1() , line.getY1());
        Point two(line.getX2() , line.getY2());

        double slope = PolygonHelper::getSlope(line);

        if (slope == 0)
        {
            if (line.getY1() == line.getY2()) slope = -1;
        }
        else {
            slope = -1/slope;
        }
        /**
         * Slope = 0 ? x1 = x2
         * Slope = -1 ? y1 = y2
         * else
         */

        int divisions = 0 , index = 0 ;
        vector<Line> bo;
        Point start = getNextPoint(one , index , {line} , startSpace );

        vector<Point> cPoints ;
        while (true)
        {
            Point next = getNextPoint(start , index , {line} , step/2 );
            if (next.getX() == INT_MAX) break;

            Line nextLine(next.getX() , next.getY() , two.getX() , two.getY());


            if ( nextLine.getLength() <= 20) break;
            divisions++;
            start = next;
            cPoints.push_back(next);
        }

        for (int i = 0; i < divisions % 4 ; ++i)
        {
            cPoints.pop_back();
        }

        divisions/=4;

        for (int i = 0; i < cPoints.size(); ++i)
        {
            Point cur = cPoints[i];
            Line cuttingLine (cur.getX() , cur.getY() , cur.getX() , cur.getY());

            if (slope == 0)
            {
                if (i%2 == 0)
                {
                    cuttingLine.setX2(cuttingLine.getX1() - 100000000);
                }
                else
                {
                    cuttingLine.setX2(cuttingLine.getX1() + 100000000);
                }
            }
            else if (slope == -1)
            {
                if (i%2 == 0)
                {
                    cuttingLine.setY2(cuttingLine.getY1() + 100000000);
                }
                else
                {
                    cuttingLine.setY2(cuttingLine.getY1() - 100000000);
                }
            }
            else{
                Point second;
                if (i%2 == 0)
                {
                    second = PolygonHelper::getSecondLinePoint(cur , slope , 10000);
                }
                else
                {
                    second = PolygonHelper::getSecondLinePoint(cur , slope , -10000);
                }
                cuttingLine.setX2(second.getX());
                cuttingLine.setY2(second.getY());
            }

            for(auto &line2 : lines)
            {
                Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line2);
                if (intersectionPoint.getX() != INT_MAX)
                {
                    cuttingLine.setX2(intersectionPoint.getX());
                    cuttingLine.setY2(intersectionPoint.getY());
                    break;
                }
            }

            minLength = min(minLength , cuttingLine.getLength());
        }

        sortCenterLines.emplace_back(minLength * divisions, line);
    }
    sort(sortCenterLines.begin(), sortCenterLines.end() , greater<>());


    //Center Line
    Line bestCenterLine = sortCenterLines[0].second;

    //Create Street with Height = centerLineHeight
    //Make bestCenterLine Double Lines
    Line shiftLine1 = PolygonHelper::shiftLine(bestCenterLine , centerLineHeight/2);
    Line shiftLine2 = PolygonHelper::shiftLine(bestCenterLine , -centerLineHeight/2);

    double dx = shiftLine1.getX2() - shiftLine1.getX1();
    double dy = shiftLine1.getY2() - shiftLine1.getY1();

    shiftLine1.setX1(shiftLine1.getX1() - dx * 5);
    shiftLine1.setY1(shiftLine1.getY1() - dy * 5);
    shiftLine1.setX2(shiftLine1.getX2() + dx * 5);
    shiftLine1.setY2(shiftLine1.getY2() + dy * 5);

    dx = shiftLine2.getX2() - shiftLine2.getX1();
    dy = shiftLine2.getY2() - shiftLine2.getY1();

    shiftLine2.setX1(shiftLine2.getX1() - dx * 5);
    shiftLine2.setY1(shiftLine2.getY1() - dy * 5);
    shiftLine2.setX2(shiftLine2.getX2() + dx * 5);
    shiftLine2.setY2(shiftLine2.getY2() + dy * 5);


    Line shift1 = shiftLine1;
    for(auto &line2 : lines)
    {
        Point intersectionPoint = PolygonHelper::getIntersectionPoint(shiftLine1 , line2);

        if (intersectionPoint.getX() != INT_MAX)
        {
            Line one (intersectionPoint.getX() , intersectionPoint.getY() , shift1.getX1() , shift1.getY1());
            Line two (intersectionPoint.getX() , intersectionPoint.getY() , shift1.getX2() , shift1.getY2());

            if (one.getLength() < two.getLength())
            {
                shiftLine1.setX1(intersectionPoint.getX());
                shiftLine1.setY1(intersectionPoint.getY());
            }
            else{
                shiftLine1.setX2(intersectionPoint.getX());
                shiftLine1.setY2(intersectionPoint.getY());
            }
        }
    }

    Line shift2 = shiftLine2;
    for(auto &line2 : lines)
    {
        Point intersectionPoint = PolygonHelper::getIntersectionPoint(shiftLine2 , line2);

        if (intersectionPoint.getX() != INT_MAX)
        {
            Line one (intersectionPoint.getX() , intersectionPoint.getY() , shift2.getX1() , shift2.getY1());
            Line two (intersectionPoint.getX() , intersectionPoint.getY() , shift2.getX2() , shift2.getY2());

            if (one.getLength() < two.getLength())
            {
                shiftLine2.setX1(intersectionPoint.getX());
                shiftLine2.setY1(intersectionPoint.getY());
            }
            else{
                shiftLine2.setX2(intersectionPoint.getX());
                shiftLine2.setY2(intersectionPoint.getY());
            }
        }
    }

    centerLines.push_back(shiftLine1);
    centerLines.push_back(shiftLine2);

    sort(centerLines.begin(), centerLines.end());

    return centerLines;
}