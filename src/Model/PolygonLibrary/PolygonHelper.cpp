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

vector<Line> PolygonHelper::getCenterLines(Polygon1 &polygon)
{
    vector<Line> centerLines;
    Point centroid = polygon.calculateCentroid();
    vector<Point> points = polygon.getPoints();
    vector<Line> lines = polygon.getLines();
    int n = (int)points.size();
    vector<Point> centerPoints ;
    vector<Point> centerPointsTOP ;
    vector<Point> centerPointsBottom ;

    for (int i = 0; i < n; ++i)
    {
        Point prev = points[i==0 ? n-1 : i-1];
        Point cur = points[i];
        Point next = points[(i+1) %n];

        if (cur.getY() > centroid.getY())
        {
            Point centerPoint(0,0);
            Point centerPointT(0,0);
            Point centerPointB(0,0);

            if (prev.getY()<= centroid.getY())
            {
                centerPoint = Point ((cur.getX()+prev.getX())/2 , (cur.getY()+prev.getY())/2);

                centerPointT = getNextPoint(centerPoint , cur , 5);
                centerPointB = getNextPoint(centerPoint , prev , 5);
            }
            else if (next.getY()<= centroid.getY())
            {
                centerPoint = Point ((cur.getX()+next.getX())/2 , (cur.getY()+next.getY())/2);

                centerPointT = getNextPoint(centerPoint , cur , 5);
                centerPointB = getNextPoint(centerPoint , prev , 5);
            }
            else{
                Line straight(cur.getX() , cur.getY() , cur.getX() , cur.getY() - 10000000);
                for(auto &line : lines)
                {
                    Point one(line.getX1() , line.getY1());
                    Point two(line.getX2() , line.getY2());

                    if (one == cur || two == cur)continue;

                    Point intersectionPoint = getIntersectionPoint(straight , line);
                    if (intersectionPoint.getX() != INT_MAX)
                    {
                        straight.setX2(intersectionPoint.getX());
                        straight.setY2(intersectionPoint.getY());
                        break;
                    }
                }
                centerPoint = Point ((straight.getX1()+straight.getX2())/2 , (straight.getY2()+straight.getY1())/2);

                centerPointT = getNextPoint(centerPoint , cur , 5);
                centerPointB = getNextPoint(centerPoint , {straight.getX2() , straight.getY2()} , 5);
            }
            centerPoints.push_back(centerPoint);
            centerPointsTOP.push_back(centerPointT);
            centerPointsBottom.push_back(centerPointB);
        }
    }

//    for (int i = 1; i < centerPoints.size(); ++i)
//    {
//        Point prev = centerPoints[ i-1];
//        Point cur = centerPoints[i];
//        centerLines.emplace_back(prev.getX() , prev.getY() , cur.getX() , cur.getY());
//    }

    for (int i = 1; i < centerPointsTOP.size(); ++i)
    {
        Point prev = centerPointsTOP[ i-1];
        Point cur = centerPointsTOP[i];
        centerLines.emplace_back(prev.getX() , prev.getY() , cur.getX() , cur.getY());
    }

    for (int i = 1; i < centerPointsBottom.size(); ++i)
    {
        Point prev = centerPointsBottom[ i-1];
        Point cur = centerPointsBottom[i];
        centerLines.emplace_back(prev.getX() , prev.getY() , cur.getX() , cur.getY());
    }
    return centerLines;
}

vector<Line> PolygonHelper::getTopLines(Polygon1 &polygon , double offsite)
{
    vector<Line> topLines;
    Point centroid = polygon.calculateCentroid();
    vector<Point> points = polygon.getPoints();
    vector<Line> lines = polygon.getLines();
    int n = (int)points.size();
    vector<Point> centerPoints ;

    for (int i = 0; i < n; ++i)
    {
        Point prev = points[i==0 ? n-1 : i-1];
        Point cur = points[i];
        Point next = points[(i+1) %n];

        if (cur.getY() > centroid.getY())
        {
            Point centerPoint(0,0);
            if (prev.getY()<= centroid.getY())
            {
                centerPoint = getNextPoint(cur , prev , offsite);
            }
            else if (next.getY()<= centroid.getY())
            {
                centerPoint = getNextPoint(cur , next , offsite);
            }
            else{
                Line straight(cur.getX() , cur.getY() , cur.getX() , cur.getY() - 10000000);

                centerPoint = getNextPoint({straight.getX1() , straight.getY1()} , {straight.getX2() , straight.getY2()} , offsite);
            }
            centerPoints.push_back(centerPoint);
        }
    }

    for (int i = 1; i < centerPoints.size(); ++i)
    {
        Point prev = centerPoints[ i-1];
        Point cur = centerPoints[i];
        topLines.emplace_back(prev.getX() , prev.getY() , cur.getX() , cur.getY());
    }
    return topLines;
}

vector<Line> PolygonHelper::getBottomLines(Polygon1 &polygon , double offsite)
{
    vector<Line> bottomLines;
    Point centroid = polygon.calculateCentroid();
    vector<Point> points = polygon.getPoints();
    vector<Line> lines = polygon.getLines();
    int n = (int)points.size();
    vector<Point> centerPoints ;

    for (int i = 0; i < n; ++i)
    {
        Point prev = points[i==0 ? n-1 : i-1];
        Point cur = points[i];
        Point next = points[(i+1) %n];

        if (cur.getY() < centroid.getY())
        {
            Point centerPoint(0,0);
            if (prev.getY()>= centroid.getY())
            {
                centerPoint = getNextPoint(cur , prev , offsite);
            }
            else if (next.getY() >= centroid.getY())
            {
                centerPoint = getNextPoint(cur , next , offsite);
            }
            else{
                Line straight(cur.getX() , cur.getY() , cur.getX() , cur.getY() + 10000000);

                centerPoint = getNextPoint({straight.getX1() , straight.getY1()} , {straight.getX2() , straight.getY2()} , offsite);
            }
            centerPoints.push_back(centerPoint);
        }
    }

    for (int i = 1; i < centerPoints.size(); ++i)
    {
        Point prev = centerPoints[ i-1];
        Point cur = centerPoints[i];
        bottomLines.emplace_back(prev.getX() , prev.getY() , cur.getX() , cur.getY());
    }
    return bottomLines;
}

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

bool PolygonHelper::isPointOnLine(const Point& p, const Line& line) {
    // Calculate the cross product to check if the point is collinear with the line
    double crossProduct = (p.getY() - line.getY1()) * (line.getX2() - line.getX1()) - (p.getX() - line.getX1()) * (line.getY2() - line.getY1());

    // Normalize the cross product with line length
    double length = std::hypot(line.getX2() - line.getX1(), line.getY2() - line.getY1());
    if (std::abs(crossProduct) > 0.1 * length) {
        return false;
    }

    // Check if the point lies within the bounding rectangle of the line segment
    double minX = min(line.getX1(), line.getX2());
    double maxX = max(line.getX1(), line.getX2());
    double minY = min(line.getY1(), line.getY2());
    double maxY = max(line.getY1(), line.getY2());

    return (p.getX() >= minX && p.getX() <= maxX && p.getY() >= minY && p.getY() <= maxY);
}

double PolygonHelper::getSlope(double x1, double x2, double y1, double y2) {
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

pair<Polygon1, Polygon1> PolygonHelper::splitPolygons(const Polygon1& polygon1, const Line &line) {
    std::vector<Point> poly1, poly2;
    std::vector<Point> intersections;
    vector<Point> points = polygon1.getPoints();

    for (size_t i = 0; i < points.size(); i++)
    {
        Point curr = points[i];
        Point next = points[(i + 1) % points.size()];

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






