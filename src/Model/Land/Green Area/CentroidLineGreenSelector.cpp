//
// Created by ahmed Ibrahim on 03-Mar-25.
//

#include "CentroidLineGreenSelector.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "src/Model/DesignGeometryManager.h"

void CentroidLineGreenSelector::select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage,double totalGreenAreas)
{
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
    cout<<"greenAreas = "<<greenAreas<<"\n";

    if (greenAreas == 0)return;

    Point centerP = outerLand.calculateCentroid();

    cout<<"Center = "<<centerP.getX()<<" "<<centerP.getY()<<"\n";
    Point minP = outerLand.minPoint();
    Point maxP = outerLand.maxPoint();
    vector<Line> lines = outerLand.getLines();
    Line cuttingLine (0, 0 , 0 , 0 );

    if (maxP.getX() - minP.getX() >= maxP.getY() - minP.getY())//Horizontal Line
    {
        cuttingLine = Line(-2000000 , centerP.getY() , 2000000 , centerP.getY());

        for (int i = 0; i < lines.size(); ++i)
        {
            Line line = lines[i];
            Point centerPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line);
            if (centerPoint.getX() == INT_MAX) continue;

            Line first (-2000000 , centerP.getY() , centerP.getX() , centerP.getY());
            Line second (centerP.getX() , centerP.getY() , 2000000 , centerP.getY());

            Point centerPoint1 = PolygonHelper::getIntersectionPoint(first , line);

            if (centerPoint1.getX() != INT_MAX)
            {
                cuttingLine.setX1(centerPoint1.getX());
//                cuttingLine.setY1(centerPoint1.getY());
                continue;
            }

            Point centerPoint2 = PolygonHelper::getIntersectionPoint(second , line);

            if (centerPoint2.getX() != INT_MAX)
            {
                cuttingLine.setX2(centerPoint2.getX());
//                cuttingLine.setY2(centerPoint2.getY());
            }
        }
    }
    else
    {
        cuttingLine = Line(centerP.getX() , -2000000 , centerP.getX() , 2000000);

        for (int i = 0; i < lines.size(); ++i)
        {
            Line line = lines[i];
            Point centerPoint = PolygonHelper::getIntersectionPoint(cuttingLine , line);
            if (centerPoint.getX() == INT_MAX) continue;

            Line first (centerP.getX() , -2000000 , centerP.getX() , centerP.getY());
            Line second(centerP.getX() , centerP.getY() , centerP.getX() , 2000000);

            Point centerPoint1 = PolygonHelper::getIntersectionPoint(first , line);

            if (centerPoint1.getX() != INT_MAX)
            {
//                cuttingLine.setX1(centerPoint1.getX());
                cuttingLine.setY1(centerPoint1.getY());
                continue;
            }

            Point centerPoint2 = PolygonHelper::getIntersectionPoint(second , line);

            if (centerPoint2.getX() != INT_MAX)
            {
//                cuttingLine.setX2(centerPoint2.getX());
                cuttingLine.setY2(centerPoint2.getY());
            }
        }
    }

    cuttingLine.print();

    double dx = (cuttingLine.getX2() - cuttingLine.getX1()) / (greenAreas+1);
    double dy = (cuttingLine.getY2() - cuttingLine.getY1()) / (greenAreas+1);

    int greenAreaCount = 0;
    while (greenAreaCount < greenAreas)
    {
        bool flag = false;
        for (int i = 1; i <= greenAreas; ++i)
        {
            if(greenAreaCount >= greenAreas)break;
            Point cut (0 , 0);
            cut.setX(cuttingLine.getX1() + i*dx);
            cut.setY(cuttingLine.getY1() + i*dy);

            for(auto &land : lands)
            {
                bool isInside = PolygonHelper::isPointInsidePolygon(cut , land);
                bool isInside2 = DesignGeometryManager::isPointInsidePolygon(land.getPoints() , cut);
                if (isInside && land.isDivisible())
                {
                    land.setDivisible(false);
                    greenAreaCount++;
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)break;
    }



//    cout<<"Outer\n";
//
//    Polygon1 newp = outerLand;
//    newp.shiftY(300);
//    newp.print();
//
//    cuttingLine.setY1(cuttingLine.getY1()+300);
//    cuttingLine.setY2(cuttingLine.getY2()+300);
//    cuttingLine.printJsonFormat();
//
//    Point centroid = newp.calculateCentroid();
//    cout<<"Center = "<<centroid.getX()<<" "<<centroid.getY()<<"\n";
}
