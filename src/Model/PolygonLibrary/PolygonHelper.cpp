//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#include "PolygonHelper.h"
#include <cmath>  // For cos, sin, and M_PI

Line PolygonHelper::extendLine(const Line &line, bool extendFromSecondPoint , double scale) {
    Line line1 (0 , 0 , 0 , 0);

    double dx = line.getX2() - line.getX1();
    double dy = line.getY2() - line.getY1();

    if (extendFromSecondPoint)
    {
        line1.setX1(line.getX2());
        line1.setY1(line.getY2());

        line1.setX2(line1.getX1() + dx*scale);
        line1.setY2(line1.getY1() + dy*scale);
    }

    else
    {
        line1.setX2(line.getX1());
        line1.setY2(line.getY1());

        line1.setX1(line1.getX2() - dx*scale);
        line1.setY1(line1.getY2() - dy*scale);
    }

    return line1;
}


Point PolygonHelper::getIntersectionPoint(const Line &line1 , const Line &line2) {

    double x1 = line1.getX1(), y1 = line1.getY1(), x2 = line1.getX2(), y2 = line1.getY2();
    double x3 = line2.getX1(), y3 = line2.getY1(), x4 = line2.getX2(), y4 = line2.getY2();
    // Calculate the determinants
    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // Check if lines are parallel (denominator is 0)
    if (denominator == 0)
    {
        return {INT_MAX , INT_MAX}; // No intersection
    }

    // Calculate the intersection point
    double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
    double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

    // Check if the intersection point is within the bounds of both line segments
    if (px + 0.1 < std::min(x1, x2) || px > std::max(x1, x2)+ 0.1 ||
        px + 0.1 < std::min(x3, x4) || px > std::max(x3, x4)+ 0.1 ||
        py + 0.1 < std::min(y1, y2) || py > std::max(y1, y2)+ 0.1 ||
        py + 0.1 < std::min(y3, y4) || py > std::max(y3, y4)+ 0.1)
    {
        return {INT_MAX , INT_MAX}; // No intersection
    }

    return Point{px, py};
}

Polygon1 PolygonHelper::getScalingPolygon(Polygon1 &mainLand , double distance)
{
    std::vector<Line> offsetLines;
    vector<Point> newPoints;
    vector<Point> curPoints = mainLand.getPoints();
    int n = (int)curPoints.size();

    // Compute offset lines
    for (int i = 0; i < n; ++i)
    {
        Point cur = curPoints[i];
        Point next = curPoints[(i + 1) % n];

        Line line (cur.getX() , cur.getY() , next.getX() , next.getY());

        double dx = line.getX2() - line.getX1();
        double dy = line.getY2() - line.getY1();

        line.setX1(line.getX1() - dx * 5);
        line.setY1(line.getY1() - dy * 5);
        line.setX2(line.getX2() + dx * 5);
        line.setY2(line.getY2() + dy * 5);
        offsetLines.push_back(shiftLine(line, distance)); // Negative X for inward shrinkage
    }

    for (int i = 0; i < n; ++i)
    {
        Point intersectionPoint = getIntersectionPoint(offsetLines[i] , offsetLines[(i + 1) % n]);
        if (intersectionPoint.getX() != INT_MAX)
        {
            newPoints.push_back(intersectionPoint);
        }
    }

    Polygon1 polygon1(newPoints);

    return polygon1;
}



//Polygon1 PolygonHelper::getScalingPolygon(Polygon1 &mainLand , double distance)
//{
//    vector<Point> newPoints;
//    vector<Point> curPoints = mainLand.getPoints();
//
//    for (int i = 0; i < curPoints.size(); ++i)
//    {
//        Point a0 = curPoints[i==0?curPoints.size()-1 : i-1];
//        Point a1 = curPoints[i];
//        Point a2 = curPoints[(i + 1) % curPoints.size()];
//
//        // Compute the direction vectors of adjacent edges
//        Point v1 = (a1 - a0).normalize(); // Edge from a0 to a1
//        Point v2 = (a2 - a1).normalize(); // Edge from a1 to a2
//
//        // Compute perpendicular vectors
//        Point perp1 = v1.perpendicular();
//        Point perp2 = v2.perpendicular();
//
//        // Compute outward direction (average of two perpendicular vectors)
//        Point offsetDir = (perp1 + perp2).normalize();
//
//        // Move the point outward by X
//        Point newPoint = a1 + offsetDir * distance;
//        newPoints.push_back(newPoint);
//    }
//
//    Polygon1 polygon1(newPoints);
//
//    return polygon1;
//}

Point PolygonHelper::getNextPoint(const Point& current, const Point& destination, double step) {
    double dx = destination.getX() - current.getX();
    double dy = destination.getY() - current.getY();
    Line line (current.getX() , current.getY() , destination.getX() , destination.getY());
    double length = line.getLength();


    if (length < step) {
        return destination;  // If the remaining distance is smaller than step, just return the destination
    }

    // Normalize the direction vector
    Point unitDirection = {dx / length, dy / length};

    // Compute the new point
    return {current.getX()  + unitDirection.getX()  * step, current.getY() + unitDirection.getY() * step};
}


bool PolygonHelper::isPointInSegment(const Point &p, const Line &line)
{
    if(abs(line.getX1() - line.getX2()) <= 0.1 || abs(line.getY1() - line.getY2()) <= 0.1 )
    {
        Line line2 (min(line.getX1() , line.getX2()) , min(line.getY1() , line.getY2()) ,
                    max(line.getX1() , line.getX2()) , max(line.getY1() , line.getY2()));

        if(abs(line.getX1() - line.getX2()) <= 0.1)
        {
            return (p.getY() <= line2.getY2() && p.getY() >= line2.getY1()    );
        }
        else
        {
            return (p.getX() <= line2.getX2() && p.getX() >= line2.getX1() );
        }

    }

    double slope = getSlope(line.getX1() , line.getX2() ,line.getY1() , line.getY2());

    double slopeWithPoint = getSlope(line.getX1() , p.getX()  , line.getY1() , p.getY() );

    // slope = MathUtils::roundingToDecimal(slope);
    // slopeWithPoint = MathUtils::roundingToDecimal(slopeWithPoint);

    if(abs(slopeWithPoint- slope) > 1)
    {
        return false;
    }

    double x = p.getX() , y = p.getY();
    double epsilon = 1e-1;
    double x1 = line.getX1() , y1 = line.getY1() , x2 = line.getX2() , y2 = line.getY2();

    if(p.getX()+ 0.1 < min(line.getX1() , line.getX2()) || p.getX() > max(line.getX1() , line.getX2())+ 0.1) return false;
    if(p.getY()+ 0.1 < min(line.getY1() , line.getY2()) || p.getY() > max(line.getY1() , line.getY2())+ 0.1) return false;

    return true;
}

bool PolygonHelper::isPointOnLine(const Point& p, const Line& line)
{
    // Calculate the cross product to check if the point is collinear with the line
    double crossProduct = (p.getY() - line.getY1()) * (line.getX2() - line.getX1()) - (p.getX() - line.getX1()) * (line.getY2() - line.getY1());

    // Normalize the cross product with line length
    double length = line.getLength();
    if (std::abs(crossProduct) > 0.1 * length) {
        return false;
    }

    double minX = min(line.getX1(), line.getX2());
    double maxX = max(line.getX1(), line.getX2());
    double minY = min(line.getY1(), line.getY2());
    double maxY = max(line.getY1(), line.getY2());

    if(p.getX()+ 0.1 < minX|| p.getX() > maxX+ 0.1) return false;
    if(p.getY()+ 0.1 < minY || p.getY() > maxY+ 0.1) return false;

    return true;
}

double PolygonHelper::getSlope(double x1, double x2, double y1, double y2) {
    if(x2 == x1) return 0;

    return (y2-y1) / (x2 - x1);
}

double PolygonHelper::getSlope(const Line &line) {
    double x1 = line.getX1(), x2 = line.getX2();
    double y1 = line.getY1(), y2 = line.getY2();
    if(x2 == x1) return 0;

    return (y2-y1) / (x2 - x1);
}

Point PolygonHelper::getSecondPointOfLineByAngle(double x, double y, double angle , double dist)
{
    double angle_rad = angle * 3.14159 / 180.0;
    double x2 = x + cos(angle_rad ) * dist ;
    double y2 = y + sin(angle_rad ) * dist ;

    x2 = MathUtils::roundingToDecimal(x2);
    y2 = MathUtils::roundingToDecimal(y2);
    return {x2 , y2};
}

bool PolygonHelper::isLineIntersectWithPolygon(Polygon1 &pol, const Line &line) {
    vector<Line> polLines = pol.getLines();

    for(auto &l : polLines)
    {
        Point intersectionPoint = getIntersectionPoint(l , line);

        if(intersectionPoint.getY() != INT_MAX)
        {
            return true;
        }
    }
    return false;
}

double PolygonHelper::getLineLength(const Point &first, const Point &second) {
    double x1 = first.getX(), y1 = first.getY();
    double x2 = second.getX(), y2 = second.getY();

    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

set<int> PolygonHelper::getBoundaryLands(Polygon1 &outerLand ,vector<Polygon1> &lands)
{
    set<int> boundary;

    Point outerCentroid = outerLand.calculateCentroid();
    vector<Line> outerLine = outerLand.getLines();

    for (int i = 0; i < lands.size(); ++i)
    {
        Point centerLand = lands[i].calculateCentroid();

        bool isBoundary = false;

        for (int j = 0; j < lands[i].getPoints().size(); ++j)
        {
            Point a1 = lands[i].getPoints()[j];
            Point a2 = lands[i].getPoints()[(j+1) % lands[i].getPoints().size()];

            for(auto &line : outerLine)
            {
                Point l1 (line.getX1() , line.getY1());
                Point l2 (line.getX2() , line.getY2());

                if(PolygonHelper::isPointOnLine(a1 , line) || PolygonHelper::isPointOnLine(a2 , line)||
                   a1 == l1 || a1 == l2 || a2 == l1 || a2 == l2)
                {
                    isBoundary = true;
                    break;
                }
            }

            if(isBoundary)break;
        }

        if(isBoundary)boundary.insert(i);
    }
    return boundary;
}

vector<Polygon1> PolygonHelper::sortPolygonByArea(vector<Polygon1> &polygons)
{
    vector<pair<double , int>> sortPolygons;
    for (int i = 0; i < polygons.size(); ++i)
    {
        double cArea = polygons[i].getArea();

        sortPolygons.emplace_back(cArea , i);
    }

    sort(sortPolygons.begin() , sortPolygons.end() , greater<>());

    vector<Polygon1> polsTemp = polygons;

    polygons.clear();

    for(auto &p : sortPolygons)polygons.push_back(polsTemp[p.second]);

    return polygons;
}

void PolygonHelper::renamePolygonsIds(Polygon1 &polygon1 , vector<Polygon1> &polygons)
{
    map<int , int> mapNewIndex;

    vector<tuple<double , double , int>> sortPolygons;


    for (int i = 0; i < polygons.size(); ++i)
    {
        Point minPoint1 = polygons[i].minPoint();
        sortPolygons.emplace_back(minPoint1.getX()+ minPoint1.getY() , minPoint1.getY() , i);
    }
    sort(sortPolygons.begin(), sortPolygons.end());


    for (int i = 0; i < polygons.size(); ++i)
    {
        int id = get<2>(sortPolygons[i]);
        polygons[id].setId(to_string(i+1));
    }
}

pair<Polygon1, Polygon1> PolygonHelper::splitPolygons(const Polygon1& polygon1, const Line &line)
{
    vector<Point> poly1, poly2;
    vector<Point> intersections;
    vector<Point> points = polygon1.getPoints();
    int n = (int)points.size();

    for (int i = 0; i < points.size(); i++)
    {
        Point curr = points[i];
        Point next = points[(i + 1) % n];

        // Check if current point is on one side of the line
        double pos = (line.getX2() - line.getX1()) * (curr.getY() - line.getY1()) -
                     (line.getY2() - line.getY1()) * (curr.getX() - line.getX1());

        if (pos >= 0) poly1.push_back(curr);
        else poly2.push_back(curr);

        // Check for intersection with the edge
        double nextPos = (line.getX2() - line.getX1()) * (next.getY() - line.getY1()) -
                         (line.getY2() - line.getY1()) * (next.getX() - line.getX1());

        if ((pos > 0 && nextPos < 0) || (pos < 0 && nextPos > 0)) {
            Point inter = getIntersection(curr, next, line);
            intersections.push_back(inter);
            poly1.push_back(inter);
            poly2.push_back(inter);
        }
    }

    return {Polygon1(poly1), Polygon1(poly2)};
}

Point PolygonHelper::getIntersection(Point A, const Point& B, const Line& line) {
    double x1 = A.getX(), y1 = A.getY(), x2 = B.getX(), y2 = B.getY();
    double x3 = line.getX1(), y3 = line.getY1(), x4 = line.getX2(), y4 = line.getY2();
    double denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (denom == 0) return A;
    double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denom;
    double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denom;
    return {px, py};
}

bool PolygonHelper::isPointInsidePolygon(const Point& point, const Polygon1& polygon)
{
    const vector<Point>& vertices = polygon.getPoints();
    int n = vertices.size();
    if (n < 3) return false;

    auto onSegment = [](const Point& p, const Point& q, const Point& r) {
        return (q.getX() <= std::max(p.getX(), r.getX()) && q.getX() >= std::min(p.getX(), r.getX()) &&
                q.getY() <= std::max(p.getY(), r.getY()) && q.getY() >= std::min(p.getY(), r.getY()));
    };

    auto orientation = [](const Point& p, const Point& q, const Point& r) {
        double val = (q.getY() - p.getY()) * (r.getX() - q.getX()) -
                     (q.getX() - p.getX()) * (r.getY() - q.getY());
        if (val == 0) return 0; // Collinear
        return (val > 0) ? 1 : 2; // Clockwise or Counterclockwise
    };

    auto doIntersect = [&](const Point& p1, const Point& q1, const Point& p2, const Point& q2) {
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        if (o1 != o2 && o3 != o4) return true;

        if (o1 == 0 && onSegment(p1, p2, q1)) return true;
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false;
    };

    Point extreme(1e9, point.getY());
    int count = 0, i = 0;
    do {
        int next = (i + 1) % n;

        if (doIntersect(vertices[i], vertices[next], point, extreme)) {
            if (orientation(vertices[i], point, vertices[next]) == 0)
                return onSegment(vertices[i], point, vertices[next]);
            count++;
        }
        i = next;
    } while (i != 0);

    return (count % 2 == 1);
}

Point PolygonHelper::getOtherLinePoint(const Point &p, double slope, double y) {
    if (slope == 0)
    {
        return {p.getX() , p.getY() + y};
    }


    double x2 = (((p.getY() - y)/slope) - p.getX()) * -1;
    return {x2 , y};
}

Point PolygonHelper::getSecondLinePoint(const Point &p, double slope, double distance)
{
    double dx = distance / std::sqrt(1 + slope * slope);
    double dy = slope * dx;

    Point p2;
    p2.setX( p.getX() + dx);
    p2.setY( p.getY() + dy);

    return p2;
}

Line PolygonHelper::shiftLine(const Line &line, double distance)
{
    double dx = line.getX2() - line.getX1();
    double dy = line.getY2() - line.getY1();
    double length = line.getLength();

    // Calculate the perpendicular vector (normal vector)
    double nx = -dy / length;  // Normalized perpendicular vector
    double ny = dx / length;

    double shiftFactor = (distance > 0) ? 1.0 : -1.0;
    double x1 = line.getX1() + shiftFactor * abs(distance) * nx;
    double y1 = line.getY1() + shiftFactor * abs(distance) * ny;

    double x2 = line.getX2() + shiftFactor * abs(distance) * nx;
    double y2 = line.getY2() + shiftFactor * abs(distance) * ny;

    // Shift the line by distance X
//    double x1 = line.getX1() + distance * nx;
//    double y1 = line.getY1() + distance * ny;
//
//    double x2 = line.getX2() + distance * nx;
//    double y2 = line.getY2() + distance * ny;

    return {x1 , y1 , x2 , y2};

}

Line PolygonHelper::getLineForPoint(const vector<Line> &lines, const Point &p)
{
    for (auto &polLine : lines)
    {
        Point one (polLine.getX1() , polLine.getY1());
        Point two (polLine.getX2() , polLine.getY2());

        if (p == one || p == two || isPointOnLine(p , polLine))
        {
            return polLine;
        }
    }
    return {INT_MAX,INT_MAX,INT_MAX,INT_MAX};
}

vector<Point> PolygonHelper::getShortestPath(Polygon1 &polygon1, const Point &start, const Point &end)
{
    vector<Point> points = polygon1.getPoints();
    int n = (int)points.size() , index1 = 0, index2 = 0;

    vector<Point> newPoints;
    bool added1 = false, added2 = false;

    for (int i = 0; i < n; ++i)
    {
        Point cur = points[i];
        Point next = points[(i+1) %n];
        Line line (cur.getX() , cur.getY() , next.getX() , next.getY());
        if(newPoints.empty() || (!newPoints.empty() && newPoints.back() != cur))newPoints.push_back(cur);
        double length1 = -1 , length2 = -1;
        if (!added1 && isPointOnLine(start , line))
        {
            Line f1(start.getX() , start.getY() , cur.getX() , cur.getY());
            length1 = f1.getLength();
            added1 = true;
        }
        if (!added2 &&isPointOnLine(end , line))
        {
            Line f1(end.getX() , end.getY() , cur.getX() , cur.getY());
            length2 = f1.getLength();
            added2 = true;
        }

        if (length1 != -1 && length2 != -1)
        {
            if (length1 < length2){

                index1 = newPoints.size();
                newPoints.push_back(start);
                index2 = newPoints.size() ;
                newPoints.push_back(end);
            }
            else {
                index2 = newPoints.size();
                newPoints.push_back(end);
                index1 = newPoints.size() ;
                newPoints.push_back(start);
            }
        }
        else if (length1 != -1)index1 = (int)newPoints.size(),newPoints.push_back(start);
        else if (length2 != -1)index2 = (int)newPoints.size(),newPoints.push_back(end);
    }

    vector<Point> ans1 , ans2;

    int i = index1;

    while (true)
    {
        ans1.push_back(newPoints[i]);
        if (i == index2) break;
        i = (i+1) % (int)newPoints.size();
    }

    i = index1;
    while (true)
    {
        ans2.push_back(newPoints[i]);
        if (i == index2) break;
        i = (i==0) ? (int)newPoints.size()-1 : (i-1);
    }

    if (ans1.size()<ans2.size()) return ans1;
    else return ans2;
}

Line PolygonHelper::clipLineToPolygon(Polygon1 &polygon1, const Line &line)
{
    vector<Line> lines = polygon1.getLines();
    vector<Point> result;

    for(auto &line2 : lines)
    {
        Point point = getIntersectionPoint(line , line2);

        if (point.getX() != INT_MAX)
        {
            result.push_back(point);
        }
    }
    if (result.size() ==2)
    {
        Line dis1 (result[0] , line.getStart());
        Line dis2 (result[0] , line.getAnEnd());

        if (dis2.getLength() < dis1.getLength())
        {
            return  {result[1] , result[0]};
        }

        return  {result[0] , result[1]};
    }

    return line;
}






