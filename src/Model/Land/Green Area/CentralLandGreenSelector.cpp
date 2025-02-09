//
// Created by ahmed Ibrahim on 09-Feb-25.
//

#include "CentralLandGreenSelector.h"
#include "src/Model/PolygonLibrary/PolygonHelper.h"
void CentralLandGreenSelector::select(Polygon1 &outerLand, vector<Polygon1> &lands, double greenAreaPercentage , double totalGreenAreas)
{
    int n = (int)lands.size();
    int greenAreas = n*greenAreaPercentage;
    cout<<"greenAreas = "<<greenAreas<<"\n";

    if (greenAreas == 0)return;

    Point outerCentroid = outerLand.calculateCentroid();
    int counterAngle = 360 / greenAreas;
    vector<Line> outerLines = outerLand.getLines();

    vector<pair<double , vector<int>>> allGreenAreasOptions;

    for (int startAngle = 0; startAngle < 90; ++startAngle)
    {
        vector<double> linesAngle ;
        int tmp = startAngle;
        for (int i = 1; i < greenAreas ; ++i)
        {
            double angle = (tmp + counterAngle);
            if (angle > 360) angle -=360;
            linesAngle.push_back(angle);
            tmp = angle;
        }

        vector<Line> lines;
        for(auto &angle : linesAngle)
        {
            Point secondP = PolygonHelper::getSecondPointOfLineByAngle(outerCentroid.getX() , outerCentroid.getY() , angle);

            Line extendLine {outerCentroid.getX() , outerCentroid.getY() , secondP.getX() , secondP.getY()};

            for(auto &mainL : outerLines)
            {
                Point intersectionPoint = PolygonHelper::getIntersectionPoint(extendLine , mainL);

                if (intersectionPoint.getX() != INT_MAX)
                {
                    Line line3(extendLine.getX1() , extendLine.getY1() , intersectionPoint.getX() , intersectionPoint.getY());

                    lines.push_back(line3);

                    break;
                }
            }

        }
        vector<int> greenAreaIndex;

        for(auto &line : lines)
        {
            vector<pair<double , int>> sortLands;
            Point center ((line.getX2() + line.getX1()) / 2 , (line.getY2() + line.getY1()) / 2);

            for (int i = 0; i < lands.size(); ++i)
            {
                Point centerLand = lands[i].calculateCentroid();
                if(PolygonHelper::isLineIntersectWithPolygon(lands[i] , line))
                {
                    double distance = PolygonHelper::getLineLength(center ,centerLand );
                    sortLands.emplace_back(distance , i);
                }
            }
            if (sortLands.empty()) continue;
            std::sort(sortLands.begin(), sortLands.end());
            int index = sortLands[0].second;
            greenAreaIndex.push_back(index);
        }
        double area = 0.0;
        for(auto &greenIndex : greenAreaIndex)
        {
            area += lands[greenIndex].getArea();
        }
        allGreenAreasOptions.emplace_back(area , greenAreaIndex);
        break;
    }

    if (allGreenAreasOptions.empty()) return;
    std::sort(allGreenAreasOptions.begin(), allGreenAreasOptions.end());

    cout<<"allGreenAreasOptions = "<<allGreenAreasOptions.size()<<"\n";

    int solIndex = lower_bound(allGreenAreasOptions.begin(), allGreenAreasOptions.end(),totalGreenAreas ,
                               [](pair<double, vector<int>> &elem , double value)
                               {
        return elem.first < value;
                               })-allGreenAreasOptions.begin();

    vector<int> greenAreaSelected = allGreenAreasOptions[solIndex].second;

    for(auto &green : greenAreaSelected)
    {
        lands[green].setDivisible(false);
    }
}
