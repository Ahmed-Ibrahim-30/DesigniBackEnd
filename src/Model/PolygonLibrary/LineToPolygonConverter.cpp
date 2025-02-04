//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#include "LineToPolygonConverter.h"
#include "src/Model/DesignGeometryManager.h"

const vector<Line> &LineToPolygonConverter::getLines() const {
    return lines;
}

void LineToPolygonConverter::setLines(const vector<Line> &lines) {
    LineToPolygonConverter::lines = lines;
}

LineToPolygonConverter::LineToPolygonConverter(const vector<Line> &lines) : lines(lines) {}

void LineToPolygonConverter::DFS(Point firstPoint,Point lastpoint,int index, set<int> &tst, vector<int> &ans , vector<vector<int>> &allAnswers)
{
    if(lastpoint == firstPoint && ans.size() > 2)
    {
        allAnswers.push_back(ans);
        return;
    }
    Line line = lines[index];
    double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

    Point one(x1 , y1);
    Point two(x2 , y2);

    for (int j = 0; j < lines.size(); ++j) {
        if (tst.count(j)) continue;

        Point three(lines[j].getX1() , lines[j].getY1());
        Point four(lines[j].getX2() , lines[j].getY2());

        if ( lastpoint == four || lastpoint == three)
        {
            tst.insert(j);
            ans.push_back(j);

            DFS(firstPoint,lastpoint== four?three : four , j , tst , ans , allAnswers);

            tst.erase(j);
            ans.pop_back();
        }
    }
}

vector<Polygon1> LineToPolygonConverter::constructPolygons(Polygon1 &innerPolygon)
{
    vector<Polygon1> polygons;

    set<set<int>> polIndex;
    vector<vector<int>> uniquePolygons;

    vector<Line> secondLines = innerPolygon.getLines();

    for (int i = lines.size()-1; i >= lines.size() - secondLines.size(); --i)
    {
        Line line = lines[i];
        double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

        set<int>tst = {i};
        vector<int> index = {i};

        Point one(x1 , y1);
        Point two(x2 , y2);
        vector<vector<int>> ans;
        DFS(one ,two, i , tst , index , ans);

        if(!polIndex.count(tst)){
            polIndex.insert(tst);
            for(auto &ii : ans)uniquePolygons.push_back(ii);

        }
        // break;
    }

    for(auto &pol : uniquePolygons)
    {
        vector<Point>points;
        for(auto &index : pol)
        {
            Line line = lines[index];

            double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

            Point one(x1 , y1);
            Point two(x2 , y2);

            if (points.empty())
            {
                points.push_back(one);
                points.push_back(two);
            }
            else{
                if (one == points[points.size()-1]) points.push_back(two);
                else if (two == points[points.size()-1])points.push_back(one);
                else if (one == points[points.size()-2])
                {
                    swap(points[points.size()-2] , points[points.size()-1]);
                    points.push_back(two);
                }
                else if (two == points[points.size()-2])
                {
                    swap(points[points.size()-2] , points[points.size()-1]);
                    points.push_back(one);
                }
            }
        }
        Polygon1 newPol(points);
        polygons.push_back(newPol);
    }


    vector<Polygon1> ans;

    for(auto &pol : polygons)
    {
        vector<Line> polLines = pol.getLines();
        set<Line> setLines;
        for(auto &line : polLines)setLines.insert(line);
        bool flag = true;

        for(auto &line : lines)
        {
            Line line3(line.getX2() , line.getY2() , line.getX1() , line.getY1());
            if(setLines.count(line) || setLines.count(line3)) continue;

            double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

            Point one(x1 , y1);
            Point two(x2 , y2);

            bool oneT = DesignGeometryManager::isPointInsidePolygon(pol.getPoints() , one) ;
            bool twoT = DesignGeometryManager::isPointInsidePolygon(pol.getPoints() , two);

            for(auto &line2 : polLines)
            {
                Point three(line2.getX1() , line2.getY1());
                Point four(line2.getX2() , line2.getY2());

                oneT |=  PolygonHelper::isPointInSegment(one , line2) || one == three || one== four;
                twoT |=  PolygonHelper::isPointInSegment(two , line2) ||two == three || two== four;
            }

            if(oneT && twoT)
            {
                flag = false;
                break;
            }

        }
        if(flag) ans.push_back(pol);
    }


    return ans;
}
