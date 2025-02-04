//
// Created by ahmed Ibrahim on 01-Jan-25.
//

#include "LandDivisionBasedOnLandArea.h"

vector<vector<Polygon1>> LandDivisionBasedOnLandArea::divideLand(const Polygon1 &land ,double area) {
    vector <Polygon1> pols;
    pols.push_back(land);
    vector<vector<Polygon1>> ans;
    divideLand( area , pols , ans);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    for(auto &pol : ans)
    {
        cout<<"********************************\n";
        for(auto &t : pol) t.print();
    }

    return ans;
}

vector<vector<Polygon1>> LandDivisionBasedOnLandArea::divideLands(vector<Polygon1> &lands, double area) {
    vector<vector<Polygon1>> ans;
    divideLand( area , lands , ans);

    cout<<"Ans Size = "<<ans.size()<<"\n";

    for(auto &pol : ans)
    {
        cout<<"********************************\n";
        for(auto &t : pol) t.print();
    }

    return ans;
}

void LandDivisionBasedOnLandArea::divideLand(double area , vector<Polygon1> &pols,
                                             vector<vector<Polygon1>> &ans) {
    if(pols.size() > 100)
    {
        ans.push_back(pols);
        return;
    }

    cout<<sol++<<"\n";
    double maxArea = -100000 ; int polygonIndex = -1;
    for (int i = 0; i < pols.size(); ++i)
    {
        if (!pols[i].isDivisible()) continue;// if land is green Area

        double area1 = pols[i].getArea();
        if(area1 > maxArea)
        {
            maxArea = area1;
            polygonIndex = i;
        }
    }

    auto polygonDivided = pols[polygonIndex];

    vector<pair<Polygon1 , Polygon1>> paiPoly = dividePolygons( polygonDivided);

    if(paiPoly.empty()) {
        ans.push_back(pols);
        return;
    }

    int index = 0;
    vector<pair<Polygon1 , Polygon1>> paiPoly2;
    for(auto &div : paiPoly){
        double area1 = div.first.getArea();
        double area2 = div.second.getArea();
        if(area1 < area || area2 < area)
        {
            continue;
        }
        paiPoly2.push_back(div);
    }
    if(paiPoly2.empty())
    {
        ans.push_back(pols);
        return;
    }

    vector<Polygon1> newPolygon ;
    for (int j = 0; j < pols.size(); ++j)
    {
        if (j == polygonIndex) continue;
        newPolygon.push_back(pols[j]);
    }

    int divIndex = 0;
    auto div = paiPoly2[divIndex];

    if (pols.size() == 1)
    {
        for(auto &sol : paiPoly2)
        {
            newPolygon.push_back(sol.first);
            newPolygon.push_back(sol.second);

            divideLand( area , newPolygon , ans);

            newPolygon.pop_back();
            newPolygon.pop_back();
        }
    }
    else
    {
        div = paiPoly2[divIndex];
        newPolygon.push_back(div.first);
        newPolygon.push_back(div.second);

        divideLand( area , newPolygon , ans);

        newPolygon.pop_back();
        newPolygon.pop_back();
    }


}



// vector<pair<Polygon1 , Polygon1>> LandDivisionBasedOnLandArea::dividePolygons(double ratioA, double ratioB , Polygon1& polygon1) {

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

//                Point intersectionPoint = getIntersectionPoint(a3 , slope==0? 0 : -1/slope , {p1.getX() , p1.getY() , p2.getX() , p2.getY()});


//                 // Point intersectionPoint = getIntersectionPoint(line ,{p1.getX() , p1.getY() , p2.getX() , p2.getY()},a3  );
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

// //        if(!ans.empty())break;
//     }


//     return ans;
// }




