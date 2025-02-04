//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#include "LandDivisionBasedOnSidesConvergence.h"
vector<vector<Polygon1>>
LandDivisionBasedOnSidesConvergence::divideLand(const Polygon1 &land, double ratioA, double ratioB, int maxDivisions) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , pols , ans);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    int index = 0;
    if(!ans.empty())
    {
        for(auto &pol : ans[0])
        {
            index++;
            // if(index != 5) continue;
            pol.print();
        }
    }

    return ans;
}

vector<vector<Polygon1>>
LandDivisionBasedOnSidesConvergence::divideLands(vector<Polygon1> &lands, double ratioA, double ratioB,
                                                 int maxDivisions) {
    vector<vector<Polygon1>> ans;
    divideLand( ratioA , ratioB , maxDivisions , lands , ans);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    int index = 0;
    if(!ans.empty())
    {
        for(auto &pol : ans[0])
        {
            index++;
            // if(index != 5) continue;
            pol.print();
        }
    }

    return ans;
}

void
LandDivisionBasedOnSidesConvergence::divideLand(double ratioA, double ratioB, int maxDivisions,
                                                vector<Polygon1> &pols,
                                                vector<vector<Polygon1>> &ans) {
    if(pols.size() >= maxDivisions){
        ans.push_back(pols);
        return;
    }
    int polygonIndex = -1;

    vector<pair<double , int>> sortPolygons;
    for (int i = 0; i < pols.size(); ++i)
    {
        double area = pols[i].getArea();

        sortPolygons.emplace_back(area , i);
    }

    sort(sortPolygons.begin() , sortPolygons.end() , greater<>());

    vector<Polygon1> polsTemp = pols;

    pols.clear();

    for(auto &p : sortPolygons)pols.push_back(polsTemp[p.second]);

    vector<tuple<double , int , int >> sortSolutions;
    vector<vector<pair<Polygon1 , Polygon1>>> solutions(pols.size());
    for (int i = 0; i < pols.size(); ++i)
    {
        if(!pols[i].isDivisible()) continue;//if land is green Area

        vector<Polygon1> newPolygons;
        for (int var = 0; var < pols.size(); ++var) {
            if(var == i) continue;
            newPolygons.push_back(pols[var]);
        }

        auto polygonDivided = pols[i];

        vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

        for (int j = 0; j < paiPoly.size(); ++j) {
            auto div = paiPoly[j];

            newPolygons.push_back(div.first);
            newPolygons.push_back(div.second);
            vector<Polygon1> pps = {div.first , div.second};

            vector<double> lengths = getPolygonsDiffLength(pps);

            sortSolutions.emplace_back(lengths.back() - lengths[0] , i , j);
            solutions[i].push_back(paiPoly[j]);

            newPolygons.pop_back();
            newPolygons.pop_back();
        }
        if(!paiPoly.empty())break;
    }

    int  second;  double t;
    int solIndex = 0;
    if(sortSolutions.empty())return;

    sort(sortSolutions.begin() , sortSolutions.end());

    tie(t , polygonIndex , second) = sortSolutions[solIndex];


    vector<Polygon1> newPolygon ;
    for (int j = 0; j < pols.size(); ++j)
    {
        if (j == polygonIndex) continue;
        newPolygon.push_back(pols[j]);
    }

    auto div = solutions[polygonIndex][second];


    newPolygon.push_back(div.first);
    newPolygon.push_back(div.second);

    divideLand( ratioA , ratioB , maxDivisions , newPolygon , ans);

    newPolygon.pop_back();
    newPolygon.pop_back();
}




// vector<pair<Polygon1 , Polygon1>> LandDivisionBasedOnSidesConvergence::dividePolygons(double ratioA, double ratioB , Polygon1 polygon1) {

//     vector<pair<Polygon1 , Polygon1>> ans;
//     int n = polygon1.getPoints().size();

//     set<Line> polLines;

//     vector<pair<double , Line>> allLines;

//     for (int i = 0; i < n; ++i) {
//         Point a1 = polygon1.getPoints()[i];
//         Point a2 = polygon1.getPoints()[(i+1)%n];

//         polLines.emplace(a1.getX() , a1.getY() , a2.getX() , a2.getY());

//         double x1 = a1.getX() , y1 = a1.getY() , x2 = a2.getX() , y2 = a2.getY();

//         double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );

//         allLines.emplace_back(length , Line(x1 , y1 , x2 , y2));
//     }

//     sort(allLines.begin() , allLines.end() , greater<>());

//     vector<pair<double , double>> ratios = {{1 , 1} , {1 , 2} , {2 , 1} , {3 , 2} , {1 , 3} , {2,5} , {1 , 4}};

//     for(auto &rat : ratios)
//     {
//         ratioA = rat.first;
//         ratioB = rat.second;

//         for (auto &l : allLines)
//         {
//             Line line = l.second;

//             double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

//             Point a1(x1 , y1);
//             Point a2(x2 , y2);

//             double slope = getSlope(x1 , x2 ,y1 , y2);

//             double pX = 0 , pY = 0;

//             if(a1.getX() >= a2.getX())
//             {
//                 pX = a1.getX() - (a1.getX() - a2.getX()) * (ratioB/(ratioA+ratioB));
//             }
//             else
//             {
//                 pX = a1.getX() + (a2.getX() - a1.getX() ) * (ratioB/(ratioA+ratioB));
//             }

//             if(a1.getY() >= a2.getY())
//             {
//                 pY = a1.getY() - (a1.getY() - a2.getY()) * (ratioB/(ratioA+ratioB));
//             }
//             else
//             {
//                 pY = a1.getY() + (a2.getY() - a1.getY() ) * (ratioB/(ratioA+ratioB));
//             }

//             Point a3 (pX , pY);
//             Point a4 (INT_MAX,INT_MAX);

//             for (int j = 0; j < n; j++) {
//                 Point p1 = polygon1.getPoints()[j];
//                 Point p2 = polygon1.getPoints()[(j + 1) % n];

//                 if(p1 == a1 || p2 == a1 || p1 == a2 || p2==a2) continue;

//                 Point intersectionPoint = getIntersectionPoint(a3 , -1/slope , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});

//                 if(intersectionPoint.getX() != INT_MAX)
//                 {
//                     a4 = intersectionPoint;
//                     break;
//                 }
//             }
//             if(a4.getX() != INT_MAX)
//             {
//                 bool flag = false;
//                 set<Line> newPolygonLine = polLines;

//                 for(auto &pLine : newPolygonLine)
//                 {
//                     if(isPointInSegment(a3 , pLine))
//                     {
//                         newPolygonLine.emplace(a3.getX() , a3.getY() , pLine.getX1() , pLine.getY1());
//                         newPolygonLine.emplace(a3.getX() , a3.getY() , pLine.getX2() , pLine.getY2());
//                         newPolygonLine.erase(pLine);
//                         flag = true;
//                         break;
//                     }
//                 }

//                 if(!flag) continue;
//                 for(auto &pLine : newPolygonLine)
//                 {
//                     if(isPointInSegment(a4 , pLine))
//                     {
//                         newPolygonLine.emplace(a4.getX() , a4.getY() , pLine.getX1() , pLine.getY1());
//                         newPolygonLine.emplace(a4.getX() , a4.getY() , pLine.getX2() , pLine.getY2());
//                         newPolygonLine.erase(pLine);
//                         flag = true;
//                         break;
//                     }
//                 }


//                 if(!flag) continue;
//                 newPolygonLine.emplace(a3.getX() , a3.getY() , a4.getX() , a4.getY());

//                 vector<Point> p1s , p2s;
//                 a3.setX(MathUtils::roundingToDecimal(a3.getX()));
//                 a3.setY(MathUtils::roundingToDecimal(a3.getY()));

//                 a4.setX(MathUtils::roundingToDecimal(a4.getX()));
//                 a4.setY(MathUtils::roundingToDecimal(a4.getY()));

//                 p1s.push_back(a3);
//                 Point curPoint = a3;
//                 while(curPoint != a4)
//                 {
//                     flag = false;
//                     for(auto &pLine : newPolygonLine)
//                     {
//                         Point p1(pLine.getX1() , pLine.getY1());
//                         Point p2(pLine.getX2() , pLine.getY2());

//                         if( (p1== a4  || p2 == a4) && p1s.size()==1) continue;

//                         if(p1==curPoint)
//                         {
//                             p1s.push_back(p2);
//                             curPoint = p2;
//                             newPolygonLine.erase(pLine);
//                             flag = true;
//                             break;
//                         }
//                         else if(p2 == curPoint)
//                         {
//                             p1s.push_back(p1);
//                             curPoint = p1;
//                             newPolygonLine.erase(pLine);
//                             flag = true;
//                             break;
//                         }
//                     }
//                     if(!flag) break;
//                 }

//                 if(!flag) continue;
//                 p2s.push_back(a3);
//                 curPoint = a3;
//                 while(curPoint != a4)
//                 {
//                     flag = false;
//                     for(auto &pLine : newPolygonLine)
//                     {
//                         Point p1(pLine.getX1() , pLine.getY1());
//                         Point p2(pLine.getX2() , pLine.getY2());

//                         if( (p1== a4  || p2 == a4) && p2s.size()==1) continue;

//                         if(p1==curPoint)
//                         {
//                             p2s.push_back(p2);
//                             curPoint = p2;
//                             newPolygonLine.erase(pLine);
//                             flag = true;
//                             break;
//                         }
//                         else if(p2 == curPoint)
//                         {
//                             p2s.push_back(p1);
//                             curPoint = p1;
//                             newPolygonLine.erase(pLine);
//                             flag = true;
//                             break;
//                         }

//                     }
//                     if(!flag) break;
//                 }
//                 if(!flag) continue;

//                 Polygon1 po1 (p1s) , po2(p2s);

//                 double area1 = po1.getArea();
//                 double area2 = po2.getArea();

//                 if(min(area1 , area2) / max(area1 , area2) < 0.5) continue;


//                 // po1.print();
//                 // po2.print();


//                 ans.emplace_back(po1 , po2);
//                 break;
//                 // return {first , second};
//             }
//         }

//         if(!ans.empty())break;
//     }


//     return ans;
// }


vector<double>LandDivisionBasedOnSidesConvergence::getPolygonsDiffLength(const vector<Polygon1> &pol)
{
    vector<double> length;

    set<Point> points;
    for(auto &p :  pol)
    {
        for(auto &o : p.getPoints())points.insert(o);
    }

    set<Line> lines;
    vector<Polygon1> newPolgpnsT = pol;
    for(auto &p :  newPolgpnsT)
    {
        for(auto &line : p.getLines())lines.insert(line);
    }

    for(auto &line : lines)
    {
        bool flag = false;
        double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();
        for(auto &p : points)
        {
            if((p.getX() == x1 && p.getY() == y1) || (p.getX() == x2 && p.getY() == y2)) continue;

            if((p.getX() == x2 && p.getY() == y1) || (p.getX() == x1 && p.getY() == y2)) continue;

            // if(isPointInSegment(p , line))
            // {
            //     flag = true;
            //     break;
            // }
        }
        if(!flag)
        {
            double lengthV = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
            length.push_back(lengthV);
        }
    }

    sort(length.begin() , length.end());


    // vector<double> length;

    // vector<Polygon1> pol2 = pol;
    // for(auto &p :  pol2)
    // {
    //     length.push_back(p.getArea());
    // }
    // sort(length.begin() , length.end());
    return length;

}


