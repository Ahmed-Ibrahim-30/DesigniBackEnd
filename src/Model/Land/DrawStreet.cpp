//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#include "DrawStreet.h"

#include "src/Model/DesignGeometryManager.h"
#include "Land.h"

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

    for(auto &city : cities)
    {
        vector<Line> inner = city.getInnerStreets();
        vector<Line> outer = city.getOuterStreets();
        inner.pop_back();
        outer.pop_back();
        city.setStreets(inner , outer);
    }


    startSpace = step1 / 2;
    vector<Line> centerTop = SplitCenterLines(startSpace , step1 , divisions ,centerLinesTop);
    //Bottoms
    startSpace = step1;
    vector<Line> centerBottoms = SplitCenterLines(startSpace , step1 , divisions , centerLinesBottom);

    vector<Line> centerCopy = centerLines;
    std::reverse(centerLines.begin(), centerLines.end());
    centerLines.pop_back();
    centerLines.pop_back();
    centerLines.insert(centerLines.end() , centerBottoms.begin() , centerBottoms.end());
    centerLines.insert(centerLines.end() , centerTop.begin() , centerTop.end());
}

vector<Line> DrawStreet::SplitCenterLines(double startSpace,double step1 , int divisions, const vector<Line> &centerLines2) {
    int index = 0;
    Point stP = {centerLines2[index].getX1() , centerLines2[index].getY1()};
    vector<Line> centerTop;
    for (int i = 0; i < divisions; ++i)
    {
        vector<Line> bottoms;
        Point next = getNextPoint(stP , index , centerLines2 , startSpace ,bottoms);
        startSpace = step1;

        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());
        bottoms.clear();

        stP = next;

        int index2 = index;
        next = getNextPoint(next , index2 , centerLines2 , circleStreetWidth , bottoms);
        bottoms.clear();

        Point next2 = getNextPoint(next , index2 , centerLines2 , startSpace - circleStreetWidth*2 , bottoms);

        centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());

        stP = getNextPoint(stP , index , centerLines2 , startSpace , bottoms);
    }
    Line l (stP.getX() , stP.getY() , centerLines2.back().getX2() , centerLines2.back().getY2());

    vector<Line> bottoms;
    stP = getNextPoint(stP , index , centerLines2 , l.getLength() + 100, bottoms);

    centerTop.insert(centerTop.end() , bottoms.begin() , bottoms.end());


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
//        if (m==3)break;
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
        innerStreet = PolygonHelper::getScalingPolygon(innerCopy , -circleStreetWidth);

        if (innerStreet.getArea() > outerStreet.getArea())
        {
            innerStreet = PolygonHelper::getScalingPolygon(innerCopy , circleStreetWidth);
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

        vector<Polygon1> homePolygons;

        double homeX = 10 , homeY = 14;

        Polygon1 home ({{0,  0},
                        {10, 0},
                        {10, 14},
                        {0,  14}});

        Polygon1 home2 ({{0,  0},
                        {14, 0},
                        {14, 10},
                        {0,  10}});

        vector<Line> homeLinesInner = innerStreet.getLines();
        Line innerBottomLine = {innerPoints[0].getX() , innerPoints[0].getY() , innerPoints[3].getX() , innerPoints[3].getY()};

        //EXTENSIONS And Border
        vector<Line> extensions = drawExtensions(polygonLines , innerBottomLine , innerPoints[0] , innerPoints[3] , innerPoints[1] , innerPoints[2] , step/2 + circleStreetWidth, centerLine , m ,divisions);
        vector<Line> homeBorder = drawHomeBorders( mainLand, homeLinesOuter , homeLinesInner , extensions ,homePolygons , centerLine, m ,divisions);
        vector<Polygon1> homes = homeSetter(homePolygons , home , home2);
        CityGrid cityGrid;
        cityGrid.setInnerStreets(homeLinesInner);
        cityGrid.setOuterStreets(homeLinesOuter);
        cityGrid.setRoadExtension(extensions);
        cityGrid.setHomeBorder(homeBorder);

        for(auto &h : homes)
        {
            vector<Line> ll = h.getLines();
            centerLines.insert(centerLines.end() , ll.begin() , ll.end());
        }

        cities.push_back(cityGrid);
    }
}

vector<Line>
DrawStreet::drawHomeBorders(Polygon1 &polygon1, vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner ,
                            vector<Line> &extensionsLine , vector<Polygon1> &homeLands ,const Line &centerLine , int divisionIndex , int divisionsCount) {

    vector<Line> homeBorderSol;
    Point start = {streetsLinesInner[0 ].getX1() , streetsLinesInner[0 ].getY1()};
    Point last = {streetsLinesInner[streetsLinesInner.size()-2].getX2() , streetsLinesInner[streetsLinesInner.size()-2].getY2()};

    Point startTOP  = {streetsLinesInner[0 ].getX2() , streetsLinesInner[0 ].getY2()};
    Point lastTOP   = {streetsLinesInner[streetsLinesInner.size()-2].getX1() , streetsLinesInner[streetsLinesInner.size()-2].getY1()};

    Line startLine (start.getX() , start.getY() , startTOP.getX() , startTOP.getY() );
    Line endLine (lastTOP.getX() , lastTOP.getY() , last.getX() , last.getY() );
    Line topLine (startTOP.getX() , startTOP.getY() , lastTOP.getX() , lastTOP.getY() );

    homeBorderSol = drawInnerHomeBorders(polygon1 , streetsLinesOuter , streetsLinesInner ,extensionsLine, homeLands, centerLine , divisionIndex , divisionsCount);


    //Build Outside Streets
    start = {streetsLinesOuter[0 ].getX1() , streetsLinesOuter[0 ].getY1()};
    last = {streetsLinesOuter[streetsLinesOuter.size()-2].getX2() , streetsLinesOuter[streetsLinesOuter.size()-2].getY2()};

    startTOP  = {streetsLinesOuter[0 ].getX2() , streetsLinesOuter[0 ].getY2()};
    lastTOP   = {streetsLinesOuter[streetsLinesOuter.size()-2].getX1() , streetsLinesOuter[streetsLinesOuter.size()-2].getY1()};

    startLine  = streetsLinesOuter[0];
    endLine = streetsLinesOuter[streetsLinesOuter.size() -2];
    topLine = streetsLinesOuter[1];
    Line bottomLine = streetsLinesOuter.back();

    vector<Line> streetsOrder = {startLine , topLine , endLine };

    int centerLineIndex = 0;
    vector<Line> outsideBorder;
    Point startPoint = {streetsOrder[0].getX1() , streetsOrder[0].getY1() };

    double length = startLine.getLength() + endLine.getLength() + topLine.getLength();

    double step = length / (int)(length/20);

    vector<Line> polygonLines = polygon1.getLines();

    for(auto &ex : extensionsLine)ex.setId("extensionsLine");
    polygonLines.insert(polygonLines.begin() , extensionsLine.begin() , extensionsLine.end());

    Line prevLine ;

    if(divisionIndex==0)
    {
        prevLine = Line(startPoint.getX() , startPoint.getY() , centerLine.getX1() , centerLine.getY1());
    }
    else if (divisionIndex == divisionsCount-1)
    {
        prevLine = Line(startPoint.getX() , startPoint.getY() , extensionsLine[0].getX1() , extensionsLine[0].getY1());
    }
    else{
        prevLine = Line(startPoint.getX() , startPoint.getY() , extensionsLine[1].getX1() , extensionsLine[1].getY1());
    }
    int index = 0;
    while(centerLineIndex < streetsOrder.size() && start != last)
    {
        index++;
        vector<Line> bottomLines;
//        if (index == 7) break;

        Point lastPoint = getNextPoint(start , centerLineIndex , streetsOrder , step , bottomLines);

        Line curLine (last.getX() , last.getY() , lastPoint.getX() , lastPoint.getY());

        if (curLine.getLength() < 20 && centerLineIndex ==2)lastPoint = last;

        if (lastPoint.getX() == INT_MAX)
        {
            break;
        }

        int lineIndex2 = centerLineIndex;

        Line eLine = streetsOrder[lineIndex2];

        Point stUp (prevLine.getX2() , prevLine.getY2());
        Point endUp (centerLine.getX2() , centerLine.getY2());

        Line oppositeLineLast = lineIndex2 == 0 ? endLine : lineIndex2 == 1 ?bottomLine : startLine;

        // Choose dx
        double dx = 10000;

        if (lastPoint != endUp)
        {
            // Determine the side of extraLine relative to mainLine
            double sideCheck = DesignGeometryManager::crossProduct(eLine.getX1(), eLine.getY1(), eLine.getX2(), eLine.getY2(), oppositeLineLast.getX1(), oppositeLineLast.getY1());


            if (sideCheck > 0)
            {
                dx = dx;  // If extraLine is on the left, move perpendicular line to the right
            }
            else
            {
                dx = -dx;  // If extraLine is on the right, move perpendicular line to the left
            }

            double slope1 = eLine.getSlope();

            if (slope1 != 0)
            {
                slope1 = -1 / slope1;
            }


            if (slope1 == 0 && eLine.getY1() == eLine.getY2())
            {
                slope1 = -1;
            }

            const Point& cur = lastPoint;
            Line cuttingLine (cur.getX() , cur.getY() , cur.getX() , cur.getY());

            if (slope1 == 0)
            {
                cuttingLine.setX2(cuttingLine.getY1() +dx);
                Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , oppositeLineLast);
                if (intersectionPoint.getX() != INT_MAX)
                {
                    cuttingLine.setY2(cuttingLine.getX1() -dx);
                }
            }
            else if (slope1 == -1)
            {
                cuttingLine.setY2(cuttingLine.getX1() +dx);

                Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , oppositeLineLast);
                if (intersectionPoint.getX() != INT_MAX)
                {
                    cuttingLine.setY2(cuttingLine.getX1() -dx);
                }
            }
            else
            {
                cuttingLine = DesignGeometryManager::getPerpendicularLine(eLine , oppositeLineLast,cur);
//                Point second = PolygonHelper::getSecondLinePoint(cur , slope1 , dx);
//                cuttingLine.setX2(second.getX());
//                cuttingLine.setY2(second.getY());
            }

            for(auto &line2 : polygonLines)
            {
                Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line2);
                if (intersectionPoint.getX() != INT_MAX)
                {
                    cuttingLine.setX2(intersectionPoint.getX());
                    cuttingLine.setY2(intersectionPoint.getY());
                    break;
                }
            }

            endUp = {cuttingLine.getX2() , cuttingLine.getY2()};
        }



        Line newLine (lastPoint.getX() , lastPoint.getY() , endUp.getX() , endUp.getY());
        homeBorderSol.emplace_back(newLine);

        Point startH = {prevLine.getX1() , prevLine.getY1()};
        Point startH1 = {prevLine.getX2() , prevLine.getY2()};

        Point endH = {newLine.getX1() , newLine.getY1()};
        Point endH1 = {newLine.getX2() , newLine.getY2()};

        Polygon1 homeLand = getHomePolygon(startH , endH ,startH1 ,  endH1 ,polygonLines , polygon1 , bottomLines);

        if (homeLand.getArea() >= 850)
        {
            Land land(homeLand);
            vector<Polygon1> ans;
            ans = land.SplitLand(2, 1 , 1  , static_cast<LandDivisionSortingStrategy>(0));

            if (ans.size()<2)homeLands.emplace_back(homeLand);
            else{
                Line cutting (INT_MAX, INT_MAX,INT_MAX,INT_MAX);

                vector<Line> l1 = ans[0].getLines();
                vector<Line> l2 = ans[1].getLines();
                set<Point> shared;

                map<Point , int> pntCounter;
                if (divisionIndex == divisionsCount-1 && index == 13)
                {
                    homeLand.print();
                    ans[0].print();
                    ans[1].print();
                }
                for(auto &p : ans[0].getPoints())
                {
                    for(auto &line : l2)
                    {
                        Point a1 (line.getX1() , line.getY1());
                        Point a2 (line.getX2() , line.getY2());
                        if (p == a1 || p== a2)
                        {
                            shared.insert(p);
                            continue;
                        }
                        double minX = min(line.getX1(), line.getX2());
                        double maxX = max(line.getX1(), line.getX2());
                        double minY = min(line.getY1(), line.getY2());
                        double maxY = max(line.getY1(), line.getY2());

                        if(p.getX()+ 0.1 < minX|| p.getX() > maxX+ 0.1) continue;
                        if(p.getY()+ 0.1 < minY || p.getY() > maxY+ 0.1) continue;
                        shared.insert(p);
                    }
                }
                for(auto &p : ans[1].getPoints())
                {
                    for(auto &line : l1)
                    {
                        Point a1 (line.getX1() , line.getY1());
                        Point a2 (line.getX2() , line.getY2());
                        if (p == a1 || p== a2)
                        {
                            shared.insert(p);
                            continue;
                        }
                        double minX = min(line.getX1(), line.getX2());
                        double maxX = max(line.getX1(), line.getX2());
                        double minY = min(line.getY1(), line.getY2());
                        double maxY = max(line.getY1(), line.getY2());

                        if(p.getX()+ 0.1 < minX|| p.getX() > maxX+ 0.1) continue;
                        if(p.getY()+ 0.1 < minY || p.getY() > maxY+ 0.1) continue;
                        shared.insert(p);
                    }
                }

//                cout<<"Shared = "<<shared.size()<<"\n";
                if (shared.size()>1)
                {
                    for(auto &p : shared)
                    {
                        if (cutting.getX1() == INT_MAX)
                        {
                            cutting.setX1(p.getX());
                            cutting.setY1(p.getY());
                        }
                        else if (cutting.getX2() == INT_MAX)
                        {
                            cutting.setX2(p.getX());
                            cutting.setY2(p.getY());
                            break;
                        }
                    }
                    homeBorderSol.emplace_back(cutting);
                    homeLands.emplace_back(ans[0]);
                    homeLands.emplace_back(ans[1]);
                }
                else{
                    homeLands.emplace_back(homeLand);
                }

            }
        }
        else{
//            vector<Point> pnt5 ;
//
//            for(auto &line : bottomLines)
//            {
//                Point one (line.getX1() , line.getY1());
//                Point two (line.getX2() , line.getY2());
//
//                pnt5.push_back(one);
//                if (!pnt5.empty() && two == pnt5[0])break;
//                pnt5.push_back(two);
//            }
//            pnt5.push_back(endUp);
//            pnt5.emplace_back(prevLine.getX2() , prevLine.getY2());


//            cout<<"hOME POINTS  = "<<homeLand.getPoints().size()<<"\n\n";
            homeLands.emplace_back(homeLand);
        }

        start = lastPoint;
        prevLine = newLine;

    }

    return homeBorderSol;
}

Polygon1 DrawStreet::getHomePolygon(const Point &start , const Point &end , const Point &start2 , const Point &end2 , const vector<Line> &mainLandLines , Polygon1 &pol , vector<Line> &bottomLines)
{
    Line startLine (start.getX(), start.getY() , start2.getX() , start2.getY());
    Line endLine (end.getX(), end.getY() , end2.getX() , end2.getY());
    Point firstOnLine , secondOnLine;
    bool flag1 = false, flag2 = false;
    for (int i = 0; i < mainLandLines.size(); ++i)
    {
        const Line& line = mainLandLines[i];

        Point intersectionPoint = PolygonHelper::getIntersectionPoint(startLine , line);
        if (intersectionPoint.getX() != INT_MAX)
        {
            if (line.getId() == "extensionsLine")
            {
                flag1 = true;
                firstOnLine = {mainLandLines[i].getX2() , mainLandLines[i].getY2()};
            }
            else{
                firstOnLine = start2;
            }
            break;
        }
    }

    for (int i = 0; i < mainLandLines.size(); ++i)
    {
        const Line& line = mainLandLines[i];

        Point intersectionPoint = PolygonHelper::getIntersectionPoint(endLine , line);
        if (intersectionPoint.getX() != INT_MAX)
        {
            if (line.getId() == "extensionsLine")
            {
                flag2 = true;
                secondOnLine = {mainLandLines[i].getX2() , mainLandLines[i].getY2()};
            }
            else{
                secondOnLine = end2;
            }
            break;
        }
    }

    vector<Point> points ;

    if (flag1 && flag2)
    {
        points = {start2 , end2};
        firstOnLine  = start2, secondOnLine = end2;
    }
    else{
        points = PolygonHelper::getShortestPath(pol , firstOnLine , secondOnLine);;
    }

//    cout<<"result Points = "<<points.size()<<"\n";
    if (secondOnLine != end2)points.push_back(end2);

    vector<Line> btmLines = bottomLines;
    for (int i = btmLines.size()-1; i  >= 0; i--)
    {
        Line line = btmLines [i];
        Point st (line.getX1() , line.getY1());
        Point st2 (line.getX2() , line.getY2());

        if (points.empty() || points.back() != st2)points.push_back(st2);
        points.push_back(st);
    }

//    points.push_back(start);
    if (firstOnLine != start2)points.push_back(start2);

    Polygon1 ans(points);

    if (points.size() ==6)
    {
//        ans.print();
    }


    return Polygon1(points);
}

vector<Polygon1> DrawStreet::homeSetter(vector<Polygon1> &lands, Polygon1 &home, Polygon1 &home2)
{
    vector<Polygon1> homes;
    for(auto &land : lands)
    {
        Polygon1 homeCopy = home;

        Line tallestLine = land.getTallestLine();

        double angle = tallestLine.getAngle();
        homeCopy.rotate(angle);

        Point centroidHome = homeCopy.calculateCentroid();

        Point centerPoint = land.calculateCentroid();

        homeCopy.transformPolygon(centerPoint.getX() -centroidHome.getX(), centerPoint.getY() -centroidHome.getY());

        if (DesignGeometryManager::isPolygonInsidePolygon(land , homeCopy))
        {
            homes.push_back(homeCopy);
        }
        else{
            homeCopy = home2;
            tallestLine = land.getTallestLine();

            angle = tallestLine.getAngle();
            homeCopy.rotate(angle);

            centroidHome = homeCopy.calculateCentroid();

            centerPoint = land.calculateCentroid();

            homeCopy.transformPolygon(centerPoint.getX() -centroidHome.getX(), centerPoint.getY() -centroidHome.getY());
            homes.push_back(homeCopy);
        }
    }
    return homes;
}

vector<Line> DrawStreet::drawExtensions(const vector<Line> &polygonLines ,const Line &bottomLine , const Point &start, const Point &end, const Point &startUp, const Point &endUp,double step ,const Line &centerLine, int divisionIndex , int divisionsCount)
{
    Line otherCenter = centerLine == centerLines[0] ? centerLines[1] : centerLines[0];
    vector<Line> extensions;
    Point centerBottom((bottomLine.getX2() + bottomLine.getX1()) / 2,(bottomLine.getY2() + bottomLine.getY1()) / 2) , centerTop;
    double reqLength = bottomLine.getLength() / 2;

    centerTop = Point ((startUp.getX()+endUp.getX())/2 , (startUp.getY()+endUp.getY())/2);

    Line centerExtension (centerBottom.getX() , centerBottom.getY() , centerTop.getX() , centerTop.getY());
//    extensions.push_back(centerExtension);

    int lineIndex = 0;

    Point nextEnd = getNextPoint(end , lineIndex , {centerLine} , step );
    lineIndex = 0;
    Point prevStart = getPrevPoint(start , lineIndex , {centerLine} , step );


    double slope1 = centerLine.getSlope();

    if (slope1 != 0)
    {
        slope1 = -1 / slope1;
    }


    Point next1UP ;
    Point next2UP ;
    if (slope1 == 0 && centerLine.getY1() == centerLine.getY2())
    {
        slope1 = -1;
    }

    vector<Point> cPoints = {nextEnd , prevStart};
    vector<Line> extBorder;
    int opp =1;
    for (int i = 0; i < cPoints.size(); ++i)
    {
        Point cur = cPoints[i];
        Line cuttingLine (cur.getX() , cur.getY() , cur.getX() , cur.getY());

        if (slope1 == 0)
        {
            cuttingLine.setY2(cuttingLine.getY1() - 100000000*opp);
        }
        else if (slope1 == -1)
        {
            cuttingLine.setX2(cuttingLine.getX1() - 100000000*opp);
        }
        else
        {
            Point second = PolygonHelper::getSecondLinePoint(cur , slope1 , 200000*opp);
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

        for(auto &line2 : polygonLines)
        {
            Point intersectionPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line2);
            if (intersectionPoint.getX() != INT_MAX)
            {
                cuttingLine.setX2(intersectionPoint.getX());
                cuttingLine.setY2(intersectionPoint.getY());
                break;
            }
        }

        extBorder.push_back(cuttingLine);
    }

    Line rightExtension =extBorder[0];
    Line leftExtension =extBorder[1];

    if (divisionIndex < divisionsCount-1)
    {
        extensions.push_back(rightExtension);
    }

    if(divisionIndex)
    {
        extensions.push_back(leftExtension);
    }

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

Point DrawStreet::getNextPoint(const Point &start, int &lineIndex, const vector<Line> &lines, double step , vector<Line> &bottomLines)
{
    if (lineIndex == lines.size())
    {
        return {INT_MAX, INT_MAX};
    }

    Point endPoint , startPoint = start;
    double mainStep = step;
    step = (int)(step*10);
    step/=10;
    while (true)
    {
        const Line& line = lines[lineIndex];
        Point destination = {line.getX2() , line.getY2()};
        Line partialLine {startPoint.getX() , startPoint.getY() , destination.getX() , destination.getY()};

        double length = partialLine.getLength();

        if (length + 0.1 < step)
        {
            lineIndex ++;
            if (lineIndex == lines.size())
            {
                cout<<"Main Step = "<<mainStep<<" "<<"Length = "<<length<<" "<<step<<"\n";
                return {INT_MAX, INT_MAX};
            }
            bottomLines.push_back(partialLine);
            step -= length;
            startPoint = {line.getX2() , line.getY2()};
        }
        else
        {
            endPoint = PolygonHelper::getNextPoint(startPoint , destination , step);
            bottomLines.emplace_back(startPoint.getX(),startPoint.getY() , endPoint.getX() , endPoint.getY());
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

vector<Line>
DrawStreet::drawInnerHomeBorders(Polygon1 &polygon1, vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner, vector<Line> &extensionsLine, vector<Polygon1> &homeLands, const Line &centerLine, int divisionIndex, int divisionsCount)
{
    vector<Line> homeBorderSol;

    Point start = {streetsLinesInner[0 ].getX1() , streetsLinesInner[0 ].getY1()};
    Point last = {streetsLinesInner[streetsLinesInner.size()-2].getX2() , streetsLinesInner[streetsLinesInner.size()-2].getY2()};
    Line bottomLine (start.getX() , start.getY() , last.getX() , last.getY() );


    Point startTOP  = {streetsLinesInner[0 ].getX2() , streetsLinesInner[0 ].getY2()};
    Point lastTOP   = {streetsLinesInner[streetsLinesInner.size()-2].getX1() , streetsLinesInner[streetsLinesInner.size()-2].getY1()};

    Line startLine (start.getX() , start.getY() , startTOP.getX() , startTOP.getY() );
    Line endLine (lastTOP.getX() , lastTOP.getY() , last.getX() , last.getY() );
    Line topLine (startTOP.getX() , startTOP.getY() , lastTOP.getX() , lastTOP.getY() );

    double minLength = min({startLine.getLength() , endLine.getLength() });

    double step = minLength / (int)(minLength/20);

    Point nextPoint1 , nextPoint2;
    int count = 0;

    while (nextPoint1 != startTOP && nextPoint2!= lastTOP)
    {
        count++;
        nextPoint1 = PolygonHelper::getNextPoint(start , startTOP , step);
        nextPoint2 = PolygonHelper::getNextPoint(last , lastTOP , step);

        vector<Point> pnt1 = {start , nextPoint1 , nextPoint2 , last};

        Polygon1 land(pnt1);

        if (land.getArea() >= 800)
        {
            Line line1 (start , last);
            Line line2 (nextPoint1 , nextPoint2);
            Point center1 = line1.getCenterPoint();
            Point center2 = line2.getCenterPoint();
            homeBorderSol.emplace_back(center1.getX() , center1.getY() , center2.getX() , center2.getY());

            pnt1 = {start , nextPoint1 , center2 , center1};
            homeLands.emplace_back(pnt1);

            pnt1 = {last , nextPoint2 , center2 , center1};
            homeLands.emplace_back(pnt1);
        }
        else if (land.getArea() >= 400)
        {
            homeLands.emplace_back(pnt1);
        }
        else break;

        if (nextPoint1 != startTOP && nextPoint2!= lastTOP)
        {
            homeBorderSol.emplace_back(nextPoint1.getX() , nextPoint1.getY() , nextPoint2.getX() , nextPoint2.getY());
        }

        start = nextPoint1;
        last = nextPoint2;
    }


    vector<Point> pnt1 = {start , startTOP , lastTOP , last};
    Polygon1 newPol(pnt1);

    if (newPol.getArea() >= 400)
    {
        homeBorderSol.emplace_back(start.getX() , start.getY() , last.getX() , last.getY());
        homeLands.emplace_back(pnt1);
        count++;
    }
    cout<<"Count = "<<count<<"\n";
    return homeBorderSol;
}
