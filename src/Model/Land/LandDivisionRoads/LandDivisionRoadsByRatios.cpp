//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#include "LandDivisionRoadsByRatios.h"

vector<vector<Polygon1>> LandDivisionRoadsByRatios::divideLand(const Polygon1 &land, const vector<double> &ratios,
                                                               LandDivisionSortingStrategy landDivisionStrategy) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;

    mainLand = land;

    divideLand( ratios, pols , ans , landDivisionStrategy);

    return ans;
}

vector<vector<Polygon1>> LandDivisionRoadsByRatios::divideLands(vector<Polygon1> &lands, const vector<double> &ratios,
                                                                LandDivisionSortingStrategy landDivisionStrategy) {
    vector<vector<Polygon1>> ans;
    divideLand( ratios , lands , ans , landDivisionStrategy);

    return ans;
}

void LandDivisionRoadsByRatios::divideLand(const vector<double> &ratios, vector<Polygon1> &pols,vector<vector<Polygon1>> &ans,
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

    vector<pair<Polygon1 , Polygon1>> paiPoly = splitPolygons( polygonDivided , ratios[curIndex] , sumOtherRatio , sqrt(mainLand.getArea() / 100000));

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

//    cout<<"Ratio --> "<<ratios[curIndex]<<" "<<sumOtherRatio<<"\t "<<selectedSolution[n-1].getArea()/selectedSolution[n-2].getArea()<<"\n";
    divideLand( ratios , selectedSolution , ans , landDivisionStrategy);
}

vector<pair<Polygon1, Polygon1>>
LandDivisionRoadsByRatios::splitPolygons(Polygon1 &polygon1, double ratio1, double ratio2 , double scalingStreet)
{
    double streetWidth1 = 8 * scalingStreet, streetWidth2 = 6 * scalingStreet, streetWidth3 = 4* scalingStreet;

    vector<pair<Polygon1 , Polygon1>> ans;
    int n = (int)polygon1.getPoints().size();

    vector< Line> allLines = polygon1.getLines();

    Point maxPolygonPoint = polygon1.maxPoint();
    Point minPolygonPoint = polygon1.minPoint();

    pair<Polygon1 , Polygon1> solution;


    for (auto &line : allLines)
    {
        double increaseFactor = 2;

        double iv = getMaxValueForLine(line , increaseFactor);

        double length = line.getLength();
        double segment = length/ increaseFactor;

        double x1 = line.getX1() , y1 = line.getY1();
        double x2 = line.getX2() , y2 = line.getY2();

        double dx = (x2 - x1)/segment;
        double dy = (y2 - y1)/segment;

        Point a1(x1 , y1);
        Point a2(x2 , y2);

        double slope = PolygonHelper::getSlope(x1 , x2 ,y1 , y2);

        double pX , pY ;

        double l = 0 , r = iv;

        while (r - l > 1e-6)
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

            if (ratioA <= ratioB) r = mid;
            else {
                l = mid;
            }

            solution = newTwoPolygons;
        }
        ans.push_back(solution);
    }

    vector<pair<double , int>> sortPols;
    for (int i = 0; i < ans.size(); ++i)
    {
        Polygon1 first  = ans[i].first;
        Polygon1 second = ans[i].second;

        vector<Line> lines = first.getLines();
        double diff = 0;
        for (int j = 0; j < lines.size(); ++j) {
            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
                    x2 = lines[j].getX2() , y2 = lines[j].getY2();

            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();

            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
            diff += abs(length - length2);
        }

        lines = second.getLines();
        for (int j = 0; j < lines.size(); ++j) {
            double x1 = lines[j].getX1() , y1 = lines[j].getY1() ,
                    x2 = lines[j].getX2() , y2 = lines[j].getY2();

            double x11 = lines[(j+1)%lines.size()].getX1() , y11 = lines[(j+1)%lines.size()].getY1() ,
                    x21 = lines[(j+1)%lines.size()].getX2() , y21 = lines[(j+1)%lines.size()].getY2();

            double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            double length2 = sqrt(((x21 - x11)*(x21 - x11)) + ((y21 - y11)*(y21 - y11)) );
            diff += abs(length - length2);
        }

        sortPols.emplace_back(diff , i);
    }

    sort(sortPols.begin() , sortPols.end());

    vector<pair<Polygon1 , Polygon1>> ans2;
    for(auto &tst : sortPols)ans2.push_back(ans[tst.second]);


    for(auto &solution1 : ans2)
    {
        Polygon1 &pol1 = solution1.first;
        Polygon1 &pol2 = solution1.second;

        Line roadLine(INT_MAX,INT_MAX,INT_MAX,INT_MAX);
        set<Point> pol1Points;
        for(auto &p : pol1.getPoints()) pol1Points.insert(p);

        for(auto &p : pol2.getPoints())
        {
            if (pol1Points.count(p))
            {
                if (roadLine.getX1() == INT_MAX)
                {
                    roadLine.setX1(p.getX());
                    roadLine.setY1(p.getY());
                }else{
                    roadLine.setX2(p.getX());
                    roadLine.setY2(p.getY());
                }
            }
        }

        Point p1 (roadLine.getX1() , roadLine.getY1());
        Point p2 (roadLine.getX2() , roadLine.getY2());

        // 0.01 * roadLine.getLength()
        double streetWidth = COUNT == 1 ? streetWidth1 : COUNT == 2 ? streetWidth2 : COUNT == 3 ?streetWidth2: streetWidth3;

        Point center1 = pol1.calculateCentroid();
        Point center2 = pol2.calculateCentroid();

        vector<Point> points1 = pol1.getPoints();
        vector<Point> points2 = pol2.getPoints();

        for (int i = 0; i < points1.size(); ++i)
        {
            Point a0 = points1[i==0?points1.size()-1 : i-1];
            Point a1 = points1[i];
            Point a2 = points1[(i + 1) % points1.size()];
            Point a3 = points1[(i + 2) % points1.size()];
            Point &p = points1[i];
            Point &pp = points1[(i + 1) % points1.size()];

            if ((a1 == p1 && a2 == p2) || (a1 == p2 && a2 == p1))
            {
                Line prev (a0.getX(),a0.getY(),a1.getX(),a1.getY());
                Line next (a2.getX(),a2.getY(),a3.getX(),a3.getY());

                p = shiftPointOnLine(prev , p , -streetWidth);
                pp = shiftPointOnLine(next , pp , streetWidth);
                break;
            }
        }

        for (int i = 0; i < points2.size(); ++i)
        {
            Point a0 = points2[i==0?points2.size()-1 : i-1];
            Point a1 = points2[i];
            Point a2 = points2[(i + 1) % points2.size()];
            Point a3 = points2[(i + 2) % points2.size()];
            Point &p = points2[i];
            Point &pp = points2[(i + 1) % points2.size()];

            if ((a1 == p1 && a2 == p2) || (a1 == p2 && a2 == p1))
            {
                Line prev (a0.getX(),a0.getY(),a1.getX(),a1.getY());
                Line next (a2.getX(),a2.getY(),a3.getX(),a3.getY());

                p = shiftPointOnLine(prev , p , -streetWidth);
                pp = shiftPointOnLine(next , pp , streetWidth);
                break;
            }
        }

        pol1.setPoints(points1);
        pol2.setPoints(points2);

    }
    return ans2;
}


double LandDivisionRoadsByRatios::getMaxValueForLine(const Line &line, double increaseFactor) {

    if (increaseFactor <= 0) return 0; // Avoid division by zero

    double x1 = line.getX1(), y1 = line.getY1();
    double x2 = line.getX2(), y2 = line.getY2();

    double length = line.getLength();
    double segment = length/ increaseFactor;

    double dx = (x2 - x1)/segment;
    double dy = (y2 - y1)/segment;

    cout<<"dx2 = "<<dx <<" -- > dy2 == "<<dy<<"\n";

    line.print();

    double l = 0 , r = 1e9 , ans = 0;
    while (r - l > 1e-6)
    {
        double mid = (l+r)/2;
        double x = x1 + mid*dx;
        double y = y1 + mid*dy;

        cout<<"X = "<<x<<" Y = "<<y<<" MID = "<<mid << "L = "<<l<<" R = "<<r<<"\n";

        bool inBounds = ((x1 <= x2 && x <= x2) || (x1 > x2 && x >= x2)) &&
                        ((y1 <= y2 && y <= y2) || (y1 > y2 && y >= y2));

        if (inBounds)l = mid , ans = mid;
        else r = mid ;
    }
    return ans;
}

LandDivisionRoadsByRatios::LandDivisionRoadsByRatios(double divisionArea, double externalRoad, double centralRoad,
                                                     double circularStreet, double landDepth, double streetCut)
        : LandDivisionRoads(divisionArea, externalRoad, centralRoad, circularStreet, landDepth, streetCut) {}
