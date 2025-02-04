//
// Created by ahmed Ibrahim on 14-Jan-25.
//

#include "Land.h"

Land::Land(const Polygon1 &land) : land(land) {

    landNetwork = new LandNetworkBySpacing();



    vector<Point>pnt1 = {
        {418.8, 20},
        {418.8, 602.5},
        {492, 749},
        {537.5, 726.2},
        {537.5, 20},
    };

    vector<Point>pnt2 = {
        {418.8, 20},
        {418.8, 602.5},
        {339.6, 444.1},
        {339.6, 20},
    };

    Polygon1 p1(pnt1) , p2(pnt2);

    vector<Polygon1> subLands = {p1 , p2};

    // vector<Polygon1> lands = landNetwork->buildLandNetwork(land , subLands);

}

vector<Polygon1> Land::SplitLand(double minSubArea)
{
    landDivision = new LandDivisionBasedOnLandArea();

    vector<vector<Polygon1>>pols = landDivision->divideLand(land , minSubArea);

    // subLand = buildRoads(pols[0]);

    cout<<"Sizes = "<<subLand.size()<<"\n";

    return pols[0];
}

vector<Polygon1> Land::SplitLand(const Design &design) {
    LOG(LogLevel::Debug , "Entry");
    landDivision = new LandDivisionBasedOnInnerDesign();
    Design newDesign = design;
    // newDesign.scaleDesign(100);
    Polygon1 innerHome (DesignOutlines::getRoofPoints(newDesign));
    // innerHome.setPoints(innerHome.scalePolygon(5));
    // innerHome.print();

   vector<vector<Polygon1>>pols = landDivision->divideLand(land , innerHome);

   // subLand = buildRoads(pols[0]);
   subLand = pols[0];

   // const Polygon1& inner = innerHome;
   // vector<Polygon1> ans = setHomesInsideSubLands(inner , 0);

   // cout<<"Sizes = "<<pols[0].size()<<"\n";
   return subLand;
}

vector<Polygon1> Land::SplitLand(int divisions, int ratioA, int ratioB) {
    landDivision = new LandDivisionBasedOnSidesConvergence();

    vector<vector<Polygon1>> pols = landDivision->divideLand(land , ratioA , ratioB , divisions);

    // subLand = buildRoads(pols[0]);

    cout<<"Sizes = "<<subLand.size()<<"\n";

    return pols[0];
}

vector<Polygon1> Land::SplitLands(vector<Polygon1> &curPolygons, double minSubArea) {
    landDivision = new LandDivisionBasedOnLandArea();

    vector<vector<Polygon1>>pols = landDivision->divideLands(curPolygons , minSubArea);

    // subLand = buildRoads(pols[0]);

    cout<<"Sizes = "<<subLand.size()<<"\n";

    return pols[0];
}

vector<Polygon1> Land::SplitLands(vector<Polygon1> &curPolygons, const Design &design) {
    landDivision = new LandDivisionBasedOnInnerDesign();
    Design newDesign = design;
    // newDesign.scaleDesign(100);
    Polygon1 innerHome (DesignOutlines::getRoofPoints(newDesign));
    // innerHome.setPoints(innerHome.scalePolygon(5));
    // innerHome.print();

    vector<vector<Polygon1>>pols = landDivision->divideLands(curPolygons , innerHome);

    // subLand = buildRoads(pols[0]);
    subLand = pols[0];

    const Polygon1& inner = innerHome;
    vector<Polygon1> ans = setHomesInsideSubLands(inner , 0);

    // cout<<"Sizes = "<<pols[0].size()<<"\n";
    return ans;
}

vector<Polygon1> Land::SplitLands(vector<Polygon1> &curPolygons, int divisions, int ratioA, int ratioB) {
    landDivision = new LandDivisionBasedOnSidesConvergence();

    vector<vector<Polygon1>> pols = landDivision->divideLands(curPolygons , ratioA , ratioB , divisions);

    // subLand = buildRoads(pols[0]);

    cout<<"Sizes = "<<subLand.size()<<"\n";

    return pols[0];
}

vector<Polygon1> Land::buildRoads(vector<Polygon1> &subLands) {
    // landNetwork = new LandNetworkByScaling();
    landNetwork = new LandNetworkBySpacing();

    vector<Polygon1> lands = landNetwork->buildLandNetwork(land , subLands);

    return lands;
}

vector<Polygon1> Land::setHomesInsideSubLands(const Polygon1 &home , int greenAreaLand)
{
    vector<Polygon1> ans;

    LOG(LogLevel::Debug , "Print Lands");

    map<int , map<Point , bool>> isPointShared = storePointsShared(subLand);

    vector<pair<int , int>> sortPolygons;

    for (int i = 0; i < subLand.size(); ++i)
    {
        int ans = 0;
        for(auto &p : isPointShared[i])
        {
            ans+=p.second;
        }
        if(ans == subLand[i].getPoints().size()) ans+=100000000;
        sortPolygons.emplace_back(ans , i);
    }
    sort(sortPolygons.begin() , sortPolygons.end() , greater<>());



    vector<Polygon1> subCopy = subLand;
    subLand.clear();
    map<int , int> mapPolToPointsShared;
    for (int i = 0; i < sortPolygons.size(); ++i) {
        int index = sortPolygons[i].second;
        mapPolToPointsShared[subLand.size()] = sortPolygons[i].first;
        subLand.push_back(subCopy[index]);
    }


    double preferDegree = 0;
    for (int i = 0; i < subLand.size(); ++i) {
        vector<Line> lines = subLand[i].getLines();
        for (const auto & line : lines) {
            double dX = line.getX2() - line.getX1();
            double dY = line.getY2() - line.getY1();

            double angle = atan2(dY, dX);
            double angleInDegrees = angle * (180.0 / M_PI);

            preferDegree =  angleInDegrees;
        }
    }
    if (preferDegree < 0) preferDegree +=360;

    LOG(LogLevel::Debug , "preferDegree = "+ to_string(preferDegree));

    for (int i = 0; i < subLand.size(); ++i){
        ans.push_back(subLand[i]);
        subLand[i].print();
    }

    for (int i = 0; i < subLand.size(); ++i){
        if(!subLand[i].isDivisible()) continue;
        ans.push_back(setHomesInsideLand(home , subLand[i] , preferDegree));
    }

    return ans;
}

Polygon1 Land::setHomesInsideLand(const Polygon1 &home , const Polygon1 &curLand1 , double preferDegree ) {
    Polygon1 curLand = curLand1;

    Polygon1 newPol = home;
    newPol.rotate(preferDegree);
    bool valid = DesignGeometryManager::positionPolygonInsideAnother(curLand , newPol);

    if (valid)
    {
        newPol.print();
        return newPol;
    }

    for (int j = 0 ; j <= 180; j+= 15) {
        newPol = home;
        if(j != 0)newPol.rotate(j);
        valid = DesignGeometryManager::positionPolygonInsideAnother(curLand , newPol);

        if(valid)
        {
            newPol.print();
            return newPol;
        }
    }

    // cout<<"NOT VALID\n";

    return home;
}

map<int , map<Point , bool>> Land::storePointsShared(const vector<Polygon1> &polys)
{
    map<int , map<Point , bool>> isPointShared;
    vector<Polygon1> pol = polys;

    for(int p1 = 0; p1 < pol.size() ; p1++)
    {
        Polygon1 c1 = pol[p1];
        vector<Point> points1 = c1.getPoints();

        for (int p2 = p1+1; p2 < pol.size(); ++p2)
        {
            int index2 = p2;
            Polygon1 c2 = pol[index2];
            vector<Point> points2 = c2.getPoints();

            for (int i = 0; i < points1.size(); ++i)
            {
                Point a1 = points1[i];
                Point a2 = points1[(i+1)%points1.size()];


                for (int j = 0; j < points2.size(); ++j)
                {
                    Point a3 = points2[j];
                    Point a4 = points2[(j+1)%points2.size()];

                    bool isLineInsideLine = (a1 == a3 && a2 == a4) || (a1 == a4 && a2 == a3)
                                            || (a2 != a3 && a2 != a4 && LandNetwork::isPointInSegment(a2 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
                                            (a1 != a3 && a1 != a4 && LandNetwork::isPointInSegment(a1 , {a3.getX() , a3.getY() , a4.getX() , a4.getY() })) ||
                                            (a3 != a1 && a3 != a2 && LandNetwork::isPointInSegment(a3 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() })) ||
                                            (a4 != a1 && a4 != a2 && LandNetwork::isPointInSegment(a4 , {a1.getX() , a1.getY() , a2.getX() , a2.getY() }));

                    if(isLineInsideLine)
                    {
                        isPointShared[p1][a1] = true;
                        isPointShared[p2][a3] = true;
                    }
                }
            }
        }

    }

    return isPointShared;
}


