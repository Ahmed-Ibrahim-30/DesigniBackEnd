//
// Created by ahmed Ibrahim on 16-Jan-25.
//

#ifndef DESIGNI_DESIGNOUTLINES_H
#define DESIGNI_DESIGNOUTLINES_H
#include "Design.h"

class DesignOutlines {
private:
    static vector<Line> getRectangleOutSideLines(const vector<Room> &recs){
        vector<Line> outSideLines;
        map<double, set<double>> xValues, yValues;
        for (auto &rec: recs) {
            if (rec.getX1() == rec.getX2())continue;
            if (rec.getY1() == rec.getY2())continue;
            xValues[rec.getX1()].insert(rec.getY1());
            xValues[rec.getX1()].insert(rec.getY2());
            xValues[rec.getX2()].insert(rec.getY1());
            xValues[rec.getX2()].insert(rec.getY2());

            yValues[rec.getY1()].insert(rec.getX1());
            yValues[rec.getY1()].insert(rec.getX2());
            yValues[rec.getY2()].insert(rec.getX1());
            yValues[rec.getY2()].insert(rec.getX2());
        }
        for (auto &x: xValues) {
            vector<double> yValue;
            yValue.reserve(x.second.size());
            for (auto &y: x.second)yValue.push_back(y);
            for (int y = 0; y < yValue.size() - 1; ++y) {
                outSideLines.emplace_back(x.first, yValue[y], x.first, yValue[y + 1]);
            }
        }
        for (auto &y: yValues) {
            vector<double> xValue;
            xValue.reserve(y.second.size());
            for (auto &x: y.second)xValue.push_back(x);
            for (int x = 0; x < xValue.size() - 1; ++x) {
                outSideLines.emplace_back(xValue[x], y.first, xValue[x + 1], y.first);
            }
        }
        return outSideLines;
    }
    static vector<Line>getOutSideLines(const vector<Room> &rectangles){
        vector<Line> outSideLines;
        vector<Line> allRectangleLines = getRectangleOutSideLines(rectangles);
        for (auto &line: allRectangleLines) {
            double x1 = line.getX1(), y1 = line.getY1();
            double x2 = line.getX2(), y2 = line.getY2();
            int counts = 0;
            for (auto &rec: rectangles) {
                if (line.getY1() == line.getY2()) {//y1 = y2
                    if ((y1 == rec.getY1() || y1 == rec.getY2()) &&
                        (x1 >= rec.getX1() && x2 <= rec.getX2())) {
                        counts++;
                    }
                }
                if (line.getX1() == line.getX2()) {//x1 = x2
                    if ((x1 == rec.getX1() || x1 == rec.getX2()) &&
                        (y1 >= rec.getY1() && y2 <= rec.getY2())) {
                        counts++;
                    }
                }
            }
            if (counts == 1)outSideLines.push_back(line);
        }
        return outSideLines;
    }
    static std::vector<Line> clockwiseSort(const std::vector<Line>& lines_){
        vector<Line> v;
        set<pair<double, double>> vis;
        v.emplace_back(lines_.front());
        while (true) {
            if (v.size() == lines_.size())break;
            auto pi = findAnotherLineConnected(v.back(), lines_, vis);
            if (pi.second == 1) {
                vis.emplace(pi.first.getX1(), pi.first.getY1());
            } else if (pi.second == 2) {
                vis.emplace(pi.first.getX1(), pi.first.getY1());
            } else if (pi.second == 3) {
                vis.emplace(pi.first.getX2(), pi.first.getY2());
            } else if (pi.second == 4) {
                vis.emplace(pi.first.getX2(), pi.first.getY2());
            }
            v.emplace_back(pi.first);
        }
        return v;
    }
    static pair<Line,int> findAnotherLineConnected(Line l, const vector<Line> &lines_,const set<pair<double,double>>&vis){
        for (const auto &line: lines_) {
            if (line.getX1() == l.getX1() && line.getX2() == l.getX2() && line.getY1() == l.getY1() && line.getY2() == l.getY2())continue;
            if (line.getX1() == l.getX1() && line.getY1() == l.getY1() && !vis.count(make_pair(line.getX1(), line.getY1()))) {//p1==p1
                return {line, 1};
            } else if (line.getX1() == l.getX2() && line.getY1() == l.getY2() && !vis.count(make_pair(line.getX1(), line.getY1()))) {//p1==p2
                return {line, 2};
            } else if (line.getX2() == l.getX1() && line.getY2() == l.getY1() && !vis.count(make_pair(line.getX2(), line.getY2()))) {//p2==p1
                return {line, 3};
            } else if (line.getX2() == l.getX2() && line.getY2() == l.getY2() && !vis.count(make_pair(line.getX2(), line.getY2()))) {//p2==p2
                return {line, 4};
            }
        }
        return {};
    }
// Calculate cross product of vectors (p1, p2) and (p1, p3)
    static double crossProduct(const Point& p1, const Point& p2, const Point& p3){
        return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY()) - (p3.getX() - p1.getX()) * (p2.getY()- p1.getY());
    }
// Check if the Points are ordered in clockwise direction
    static bool isClockwise(const std::vector<Point>& points){
        int n = points.size();
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            sum += crossProduct(points[i], points[(i + 1) % n], points[(i + 2) % n]);
        }
        return sum < 0.0;
    }
    static vector<Point> sortingPoints(vector<Line> lines_){
        vector<Point> points;
        for (const auto &l: lines_) {
            points.emplace_back(l.getX1(), l.getY1());
            points.emplace_back(l.getX2(), l.getY2());
        }

        vector<Point> points_Finals;
        if (points[1] != points[2] && points[1] != points[3]) {
            points_Finals.emplace_back(points[1]);
            points_Finals.emplace_back(points[0]);
            swap(points[0], points[1]);
        } else {
            points_Finals.emplace_back(points[0]);
            points_Finals.emplace_back(points[1]);
        }
        auto p2 = points[2];
        auto p3 = points[3];
        if (p2.getX() == points[1].getX() && p2.getY() == points[1].getY()) {
            points_Finals.emplace_back(p2);
            points_Finals.emplace_back(p3);
        } else {
            points_Finals.emplace_back(p3);
            points_Finals.emplace_back(p2);
            swap(points[3], points[2]);
        }
        for (int i = 4; i < points.size(); i += 2) {
            auto curr = points[i];
            auto next = points[i + 1];
            if (curr.getX() == points[i - 1].getX() && curr.getY() == points[i - 1].getY()) {
                points_Finals.emplace_back(curr);
                points_Finals.emplace_back(next);
            } else {
                points_Finals.emplace_back(next);
                points_Finals.emplace_back(curr);
                swap(points[i + 1], points[i]);
            }
        }
        vector<Point> ff;
        ff.emplace_back(points_Finals.front());
        for (int i = 1; i <= points_Finals.size(); ++i) {
            if (i % 2 == 1)ff.emplace_back(points_Finals[i]);
        }
        auto first = ff.front();
        auto last = ff.back();
        if (first != last)ff.push_back(first);
        //insure unticlock wise
        if (isClockwise(ff)) {
            reverse(ff.begin(), ff.end());
        }
        return ff;
    }

public:
    static vector<Point> getRoofPoints(const vector<Room>& recs){
        vector<Point> points;
        if (recs.empty())return points;
        auto OuterLines = getOutSideLines(recs);

        auto sort = clockwiseSort(OuterLines);
        points = sortingPoints(sort);

        return points;
    }

    static vector<Point> getRoofPoints(const Design &design){
        const vector<Room>& recs = design.getRooms();
        vector<Point> points;
        if (recs.empty())return points;
        auto OuterLines = getOutSideLines(recs);

        auto sort = clockwiseSort(OuterLines);
        points = sortingPoints(sort);

        return points;
    }
};


#endif //DESIGNI_DESIGNOUTLINES_H
