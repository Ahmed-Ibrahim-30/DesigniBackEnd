//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#include "LandDivisionBasedOnRatios.h"

vector<vector<Polygon1>> LandDivisionBasedOnRatios::divideLand(const Polygon1 &land, const vector<double> &ratios,
                                                               LandDivisionSortingStrategy landDivisionStrategy) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;

    mainLand = land;

    divideLand( ratios, pols , ans , landDivisionStrategy);

    return ans;
}

vector<vector<Polygon1>> LandDivisionBasedOnRatios::divideLands(vector<Polygon1> &lands, const vector<double> &ratios,
                                                                LandDivisionSortingStrategy landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( ratios , lands , ans , landDivisionStrategy);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    if(!ans.empty())
    {
        for(auto &pol : ans[0])
        {
            pol.print();
        }
    }

    return ans;
}

void LandDivisionBasedOnRatios::divideLand(const vector<double> &ratios, vector<Polygon1> &pols,vector<vector<Polygon1>> &ans,
                                           LandDivisionSortingStrategy landDivisionStrategy){

    int divisions = (int)ratios.size();

    if(pols.size() >= divisions)
    {
        ans.push_back(pols);
        return;
    }

    int curIndex = (int)pols.size() - 1;

    double sumOtherRatio = 0;
    for (int i = curIndex+1; i < ratios.size(); ++i) {
        sumOtherRatio += ratios[i];
    }

    SortLandDivisions *sortLandDivisions ;
    switch (landDivisionStrategy)
    {
        case MinimizingLengthVariance:
            sortLandDivisions = new SortLandDivisionsByMinimizingLengthVariance();
            break;
        case MinimizeSmallDimensions:
            sortLandDivisions = new SortLandDivisionsByMinimizedDimensions();
            break;
        case MinimizeAcuteAngles:
            sortLandDivisions = new SortLandDivisionsByMinimizedAcuteAngles();
            break;
        case MinimizeLandConnections:
            sortLandDivisions = new SortLandDivisionsByMinimizedConnections();
            break;
        case MaximizeLandConnections:
            sortLandDivisions = new SortLandDivisionsByMaximizedConnections();
            break;
    }


    vector<vector<Polygon1>> possibleDivisions;

    vector<Polygon1> newPolygons = pols;
    newPolygons.pop_back();

    auto polygonDivided = pols.back();

    vector<pair<Polygon1 , Polygon1>> paiPoly = splitPolygons( polygonDivided , ratios[curIndex] , sumOtherRatio);

    for (const auto& div : paiPoly)
    {
        newPolygons.push_back(div.first);
        newPolygons.push_back(div.second);

        possibleDivisions.push_back(newPolygons);

        newPolygons.pop_back();
        newPolygons.pop_back();
    }

    if(possibleDivisions.empty())
    {
        ans.push_back(pols);
        return;
    }

    possibleDivisions = sortLandDivisions->sortDivisions(possibleDivisions , ratios[curIndex] , sumOtherRatio);

    vector<Polygon1> selectedSolution = possibleDivisions[0];
    int n = (int)selectedSolution.size();

    if (selectedSolution[n-1].getArea() < selectedSolution[n-2].getArea() && ratios[curIndex] < sumOtherRatio)
    {
        swap(selectedSolution[n-1] , selectedSolution[n-2]);
    }

    cout<<"Ratio --> "<<ratios[curIndex]<<" "<<sumOtherRatio<<"\t "<<selectedSolution[n-1].getArea()/selectedSolution[n-2].getArea()<<"\n";
    divideLand( ratios , selectedSolution , ans , landDivisionStrategy);
}

//vector<pair<Polygon1, Polygon1>>
//LandDivisionBasedOnRatios::splitPolygons(Polygon1 &polygon1, double ratio1, double ratio2)
//{
//    vector<pair<Polygon1 , Polygon1>> ans;
//    int n = (int)polygon1.getPoints().size();
//
//    set<Line> polLines;
//
//    vector<pair<double , Line>> allLines;
//
//    Point maxPolygonPoint = polygon1.maxPoint();
//    Point minPolygonPoint = polygon1.minPoint();
//
//    double ratioA , ratioB ;
//
//    for (int i = 0; i < n; ++i)
//    {
//        Point a1 = polygon1.getPoints()[i];
//        Point a2 = polygon1.getPoints()[(i+1)%n];
//
//        polLines.emplace(a1.getX() , a1.getY() , a2.getX() , a2.getY());
//
//        double x1 = a1.getX() , y1 = a1.getY() , x2 = a2.getX() , y2 = a2.getY();
//
//        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
//
//        allLines.emplace_back(length , Line(x1 , y1 , x2 , y2));
//    }
//
//    sort(allLines.begin() , allLines.end() , greater<>());
//
//    vector<pair<double , double>> ratios = {{ratio1  , ratio2},{ratio1+1  , ratio2},{ratio1  , ratio2-1},{1 , 1} , {1 , 2}  , {3 , 2} , {1 , 3} , {2,5} , {1 , 4} , {3,4}};
//
//    for(auto &rat : ratios)
//    {
//        ratioA = rat.first;
//        ratioB = rat.second;
//
//        for (auto &l : allLines)
//        {
//            Line line = l.second;
//
//            double x1 = line.getX1() , y1 = line.getY1() ,
//                    x2 = line.getX2() , y2 = line.getY2();
//
//            Point a1(x1 , y1);
//            Point a2(x2 , y2);
//
//            double slope = PolygonHelper::getSlope(x1 , x2 ,y1 , y2);
//
//            double pX = 0 , pY = 0;
//
//            if(a1.getX() >= a2.getX())
//            {
//                pX = a1.getX() - (a1.getX() - a2.getX()) * (ratioB/(ratioA+ratioB));
//            }
//            else
//            {
//                pX = a1.getX() + (a2.getX() - a1.getX() ) * (ratioB/(ratioA+ratioB));
//            }
//
//            if(a1.getY() >= a2.getY())
//            {
//                pY = a1.getY() - (a1.getY() - a2.getY()) * (ratioB/(ratioA+ratioB));
//            }
//            else
//            {
//                pY = a1.getY() + (a2.getY() - a1.getY() ) * (ratioB/(ratioA+ratioB));
//            }
//
//            Point a3 (pX , pY);
//
//            Point a4 (pX * (maxPolygonPoint.getX() - pX + 10000),pY * (maxPolygonPoint.getY() - pY + 100000));
//
//            for (int j = 0; j < n; j++)
//            {
//                Point p1 = polygon1.getPoints()[j];
//                Point p2 = polygon1.getPoints()[(j + 1) % n];
//
//                Line line1 (p1.getX() , p1.getY() , p2.getX() , p2.getY());
//                Line line2 (p2.getX() , p2.getY() , p1.getX() , p1.getY());
//
//                if(line1 == line || line2 == line) continue;
//
//                Point intersectionPoint(0,0);
//
//                if(abs(a1.getX() - a2.getX()) > 0.1 && abs(a1.getY() - a2.getY()) > 0.1)
//                    intersectionPoint = getIntersectionPoint(a3 , -1/slope , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});
//                else{
//                    if(abs(a1.getX() - a2.getX()) <= 0.1)
//                    {
//                        a4 = Point(pX * ((maxPolygonPoint.getX() - minPolygonPoint.getX()) *100) , a3.getY());
//                    }
//                    else
//                    {
//                        a4 = Point(a3.getX() , pY * ((maxPolygonPoint.getY() - minPolygonPoint.getY()) *100));
//                    }
//
//                    intersectionPoint = PolygonHelper::getIntersectionPoint({a3.getX() , a3.getY() , a4.getX() , a4.getY()} , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});
//
//                    if(intersectionPoint.getX() == INT_MAX || (intersectionPoint != a1 && intersectionPoint != a2))
//                    {
//                        if(abs(a1.getX() - a2.getX()) <= 0.1)
//                        {
//                            a4 = Point(pX * -((maxPolygonPoint.getX() - minPolygonPoint.getX()) *100) , a3.getY());
//                        }
//                        else
//                        {
//                            a4 = Point(a3.getX() , pY * -((maxPolygonPoint.getY() - minPolygonPoint.getY()) *100));
//                        }
//                        intersectionPoint = PolygonHelper::getIntersectionPoint({a3.getX() , a3.getY() , a4.getX() , a4.getY()} , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});
//                    }
//                }
//
//
//
//                if(intersectionPoint.getX() != INT_MAX && intersectionPoint != a1 && intersectionPoint != a2)
//                {
//                    a4 = intersectionPoint;
//                    break;
//                }
//            }
//
//            Line intersectionLine(a3.getX() , a3.getY() , a4.getX() , a4.getY());
//
//            pair<Polygon1  , Polygon1> newTwoPolygons = PolygonHelper::splitPolygons(polygon1 , intersectionLine);
//
//            ans.emplace_back(newTwoPolygons.first , newTwoPolygons.second);
//            break;
//
//        }
//
//    }
//
//    vector<pair<double , int>> sortPols;
//    for (int i = 0; i < ans.size(); ++i)
//    {
//        Polygon1 first  = ans[i].first;
//        Polygon1 second = ans[i].second;
//
//        vector<Line> lines = first.getLines();
//        double diff = 0;
//        for (int j = 0; j < lines.size(); ++j) {
//            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
//                    x2 = lines[j].getX2() , y2 = lines[j].getY2();
//
//            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
//                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();
//
//            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
//            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
//            diff += abs(length - length2);
//        }
//
//        lines = second.getLines();
//        for (int j = 0; j < lines.size(); ++j) {
//            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
//                    x2 = lines[j].getX2() , y2 = lines[j].getY2();
//
//            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
//                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();
//
//            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
//            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
//            diff += abs(length - length2);
//        }
//
//        sortPols.emplace_back(diff , i);
//    }
//
//    sort(sortPols.begin() , sortPols.end());
//
//    vector<pair<Polygon1 , Polygon1>> ans2;
//    for(auto &tst : sortPols)ans2.push_back(ans[tst.second]);
//
//
//    return ans2;
//}

vector<pair<Polygon1, Polygon1>>
LandDivisionBasedOnRatios::splitPolygons(Polygon1 &polygon1, double ratio1, double ratio2)
{
    vector<pair<Polygon1 , Polygon1>> ans;
    int n = (int)polygon1.getPoints().size();

    set<Line> polLines;

    vector<pair<double , Line>> allLines;

    Point maxPolygonPoint = polygon1.maxPoint();
    Point minPolygonPoint = polygon1.minPoint();

    for (int i = 0; i < n; ++i)
    {
        Point a1 = polygon1.getPoints()[i];
        Point a2 = polygon1.getPoints()[(i+1)%n];

        polLines.emplace(a1.getX() , a1.getY() , a2.getX() , a2.getY());

        double x1 = a1.getX() , y1 = a1.getY() , x2 = a2.getX() , y2 = a2.getY();

        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );

        allLines.emplace_back(length , Line(x1 , y1 , x2 , y2));
    }

    sort(allLines.begin() , allLines.end() , greater<>());

    pair<Polygon1 , Polygon1> solution;


    for (auto &li : allLines)
    {
        Line line = li.second;
        double increaseFactor = 2;

        double iv = getMaxValueForLine(line , increaseFactor);

        cout<<"IV = " << iv<<"\n";

        double length = line.getLength();
        double segment = length/ increaseFactor;
        cout<<"LENGTH = "<<length <<" -- > segment == "<<segment<<"\n";

        double x1 = line.getX1() , y1 = line.getY1();
        double x2 = line.getX2() , y2 = line.getY2();

        double dx = (x2 - x1)/segment;
        double dy = (y2 - y1)/segment;

        cout<<"dx = "<<dx <<" -- > dy == "<<dy<<"\n";

        Point a1(x1 , y1);
        Point a2(x2 , y2);

        double slope = PolygonHelper::getSlope(x1 , x2 ,y1 , y2);

        double pX = 0 , pY = 0;

        double l = 0 , r = iv;

        while (l<=r)
        {
            double mid = (l+r)/2;

            pX = x1 + mid * dx;
            pY = y1 + mid * dy;

            Point a3 (pX , pY);

            Point a4 (pX * (maxPolygonPoint.getX() - pX + 10000),pY * (maxPolygonPoint.getY() - pY + 100000));

            for (int j = 0; j < n; j++)
            {
                Point p1 = polygon1.getPoints()[j];
                Point p2 = polygon1.getPoints()[(j + 1) % n];

                Line line1 (p1.getX() , p1.getY() , p2.getX() , p2.getY());
                Line line2 (p2.getX() , p2.getY() , p1.getX() , p1.getY());

                if(line1 == line || line2 == line) continue;

                Point intersectionPoint(0,0);

                if(abs(a1.getX() - a2.getX()) > 0.1 && abs(a1.getY() - a2.getY()) > 0.1)
                    intersectionPoint = getIntersectionPoint(a3 , -1/slope , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});
                else{
                    if(abs(a1.getX() - a2.getX()) <= 0.1)
                    {
                        a4 = Point(pX * ((maxPolygonPoint.getX() - minPolygonPoint.getX()) *100) , a3.getY());
                    }
                    else
                    {
                        a4 = Point(a3.getX() , pY * ((maxPolygonPoint.getY() - minPolygonPoint.getY()) *100));
                    }

                    intersectionPoint = PolygonHelper::getIntersectionPoint({a3.getX() , a3.getY() , a4.getX() , a4.getY()} , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});

                    if(intersectionPoint.getX() == INT_MAX || (intersectionPoint != a1 && intersectionPoint != a2))
                    {
                        if(abs(a1.getX() - a2.getX()) <= 0.1)
                        {
                            a4 = Point(pX * -((maxPolygonPoint.getX() - minPolygonPoint.getX()) *100) , a3.getY());
                        }
                        else
                        {
                            a4 = Point(a3.getX() , pY * -((maxPolygonPoint.getY() - minPolygonPoint.getY()) *100));
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

            double ratioA = area1 / area2;
            double ratioB = ratio1 / ratio2;

            if (ratioA > ratioB) r = mid;
            else {
                l = mid;
                solution = newTwoPolygons;
            }

            ans.emplace_back(newTwoPolygons.first , newTwoPolygons.second );
        }

    }

//    vector<pair<double , int>> sortPols;
//    for (int i = 0; i < ans.size(); ++i)
//    {
//        Polygon1 first  = ans[i].first;
//        Polygon1 second = ans[i].second;
//
//        vector<Line> lines = first.getLines();
//        double diff = 0;
//        for (int j = 0; j < lines.size(); ++j) {
//            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
//                    x2 = lines[j].getX2() , y2 = lines[j].getY2();
//
//            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
//                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();
//
//            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
//            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
//            diff += abs(length - length2);
//        }
//
//        lines = second.getLines();
//        for (int j = 0; j < lines.size(); ++j) {
//            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
//                    x2 = lines[j].getX2() , y2 = lines[j].getY2();
//
//            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
//                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();
//
//            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
//            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
//            diff += abs(length - length2);
//        }
//
//        sortPols.emplace_back(diff , i);
//    }
//
//    sort(sortPols.begin() , sortPols.end());

    vector<pair<Polygon1 , Polygon1>> ans2;
//    for(auto &tst : sortPols)ans2.push_back(ans[tst.second]);
//    ans2.push_back(solution);

    return ans;
}

double LandDivisionBasedOnRatios::getMaxValueForLine(const Line &line, double increaseFactor) {

    if (increaseFactor <= 0) return 0; // Avoid division by zero

    double x1  , y1 , x2 , y2;

    double length = line.getLength();
    double segment = length/ increaseFactor;

    double dx = (x2 - x1)/segment;
    double dy = (y2 - y1)/segment;

    double l = 0 , r = 1e9 , ans = 0;
    while (r - l > 1e-6)
    {
        double mid = (l+r)/2;
        double x = x1 + mid*dx;
        double y = y1 + mid*dy;

        if (x <= x2 && y<= y2)l = mid , ans = mid;
        else r = mid;
    }
    return ans;
}

