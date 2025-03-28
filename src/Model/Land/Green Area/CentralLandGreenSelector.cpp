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

    set<int> boundaryLands = PolygonHelper::getBoundaryLands(outerLand , lands);

    vector<pair<double , vector<int>>> allGreenAreasOptions;

    for (int startAngle = 0; startAngle <= 90; startAngle += 5)
    {
        vector<double> linesAngle ;
        int tmp = startAngle;
        for (int i = 0; i < greenAreas ; ++i)
        {
            double angle = (tmp + counterAngle);
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
        set<int> visited;
        int lineIndex = 1;

        cout<<"Green Areas Lines = "<<lines.size()<<"\n";

        for(auto &line : lines)
        {
            vector<pair<double , int>> sortLands;
            Point center ((line.getX2() + line.getX1()) / 2.0 , (line.getY2() + line.getY1()) / 2.0);

//                cout<<"Line -- ";
            for (int i = 0; i < lands.size(); ++i)
            {
                Point centerLand = lands[i].calculateCentroid();
                if(!visited.count(i) &&PolygonHelper::isLineIntersectWithPolygon(lands[i] , line))
                {
                    double distance = PolygonHelper::getLineLength(center ,centerLand );

                    if(boundaryLands.count(i)) distance *= 10000;

                    sortLands.emplace_back(distance , i);
//                    cout<<i+1 <<"( "<<distance<<")  ";
                }
            }
//            cout<<"\n";
            lineIndex++;
            if (sortLands.empty()) continue;
            std::sort(sortLands.begin(), sortLands.end());
            int index = sortLands[0].second;
            greenAreaIndex.push_back(index);
            visited.insert(index);
        }
        double area = 0.0;
        for(auto &greenIndex : greenAreaIndex)
        {
            area += lands[greenIndex].getArea();

            if (boundaryLands.count(greenIndex)) area += 100000;
        }
        allGreenAreasOptions.emplace_back(area , greenAreaIndex);
//        break;
    }

    if (allGreenAreasOptions.empty()) return;
    std::sort(allGreenAreasOptions.begin(), allGreenAreasOptions.end());

    cout<<"allGreenAreasOptions = "<<allGreenAreasOptions.size()<<"\n";

    int solIndex = upper_bound(allGreenAreasOptions.begin(), allGreenAreasOptions.end(), totalGreenAreas,
                                    [](double value, const std::pair<double, std::vector<int>>& elem) {
                                        return value < elem.first;  // Compare value with element's first (double)
                                    }) - allGreenAreasOptions.begin();


    if (solIndex > 0)solIndex--;

    vector<int> greenAreaSelected = allGreenAreasOptions[solIndex].second;

    for(auto &green : greenAreaSelected)
    {
        lands[green].setDivisible(false);
    }
}
