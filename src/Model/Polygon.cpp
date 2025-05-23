//
// Created by ahmed Ibrahim on 25-Dec-24.
//

#include "Polygon.h"

Polygon1::Polygon1(const vector<Point> &points_ , bool divisible_) : points(points_) , divisible(divisible_){

    if(points.size() > 1 && points[points.size()-1] == points[0])points.pop_back();

    if(points.size() >1)
    {
        for (int i = 0; i < points.size(); ++i) {
            Point cur = points[i];
            Point next = points[(i+1) %points.size()];

            if (cur.getX() - next.getX() > 0.1 && cur.getX() - next.getX() <= 0.2)
            {
                next.setX(cur.getX());
            }
            else if (next.getX() - cur.getX() > 0.1 && next.getX() - cur.getX() <= 0.2)
            {
                cur.setX(next.getX());
            }

            if (cur.getY() - next.getY() > 0.1 && cur.getY() - next.getY() <= 0.2)
            {
                next.setY(cur.getY());
            }
            else if (next.getY() - cur.getY() > 0.1 && next.getY() - cur.getY() <= 0.2)
            {
                cur.setY(next.getY());
            }
        }
    }


//    Point start = points[0];
//    Point end = points.back();

//    if (start.getY() < end.getY())reverse(points.begin(), points.end());

}

const vector<Point> &Polygon1::getPoints() const {
    return points;
}

void Polygon1::setPoints(const vector<Point> &points_) {
    Polygon1::points = points_;

    if(points.size()>1 && points[points.size()-1] == points[0])points.pop_back();

    // if(points.size() >1)
    // {
    //     vector<Point> newP ;
    //     newP.push_back(points[0]);
    //     newP.push_back(points[1]);

    //     for (int i = 2; i < points.size(); ++i)
    //     {
    //         if(abs(points[i].getX() - points[i-1].getX()) <= 0.1 && abs(points[i-1].getX() - points[i-2].getX()) <= 0.1)
    //         {
    //             newP.pop_back();
    //         }
    //         else if(abs(points[i].getY() - points[i-1].getY()) <= 0.1 && abs(points[i-1].getY() - points[i-2].getY()) <= 0.1)
    //         {
    //             newP.pop_back();
    //         }
    //         newP.push_back(points[i]);
    //     }
    //     points = newP;
    // }


}

double Polygon1::getArea()
{
    double area = 0.0;

    for (int i = 0; i < points.size(); ++i)
    {
        int nextIndex = (i+1) % (int) points.size();
        double a1 = points[i].getX() * points[nextIndex].getY();
        double a2 = points[nextIndex].getX() * points[i].getY();

        area += (a1 - a2);
    }

    return abs(area) / 2.0;
}

vector<Point> Polygon1::getOutsidePoints() {
    vector<Point> outsidePoints;
    int n = (int)points.size();

    for (int i = 0; i < points.size(); ++i)
    {
        Point a1 = points[i];
        Point a2 = points[(i+1)%n];

        for (double t = 0; t <= 1; t+=0.1) {
            Point curPoint (a1.getX() + (t*(a2.getX()-a1.getX())) , a1.getY() + (t*(a2.getY()-a1.getY())));
            outsidePoints.push_back(curPoint);
        }
    }

    return outsidePoints;
}

void Polygon1::print()
{
    double minX = 1000000 , minY = 100000;
    vector<Point> poi1 = points;

    for(auto &p : poi1)
    {
        minX = min(minX , p.getX());
        minY = min(minY , p.getY());
    }

    cout<<"minX = "<<minX<<" "<<minY<<"\n";

    for(auto &p : poi1)
    {
        if (minX < 0)p.setX(p.getX() + (minX * -1));
        if (minY < 0)p.setY(p.getY() + (minY * -1));
    }

    // cout<<"Polygon -- .. \n";
    int n = (int)poi1.size();
    for (int i = 0; i < poi1.size(); ++i) {
        Point a1 = poi1[i];
        Point a2 = poi1[(i+1)%n];
        cout << "{ x1:" << a1.getX() << ", y1:" << a1.getY() << ", x2:" << a2.getX() << ", y2:" << a2.getY()<< "},\n";
    }

//    for(auto &p : poi1)
//    {
//        cout<<"{"<<p.getX() <<","<<p.getY()<<"},";
//    }
}

double Polygon1::getDifferenceBetweenMINMAXSideLength() {
    double diffLength = 0;
    vector<double> lengthVec;
    for (int i = 0; i < points.size(); ++i)
    {
        Point p1 = points[i];
        Point p2 = points[(i+1) % points.size()];
        double x1 = p1.getX() , y1 = p1.getY() , x2 = p2.getX() , y2 = p2.getY();

        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
        lengthVec.push_back(length);
    }
    sort(lengthVec.begin() , lengthVec.end());

    for (int i = 1; i < lengthVec.size(); ++i) {
        diffLength += lengthVec[i] - lengthVec[i-1];
    }

    diffLength += lengthVec.back() - lengthVec[0];

    return diffLength;
}

double Polygon1::getMAXSideLength()
{
    double maxSide = -10000000;
    for (int i = 0; i < points.size(); ++i)
    {
        Point p1 = points[i];
        Point p2 = points[(i+1) % points.size()];
        double x1 = p1.getX() , y1 = p1.getY() , x2 = p2.getX() , y2 = p2.getY();

        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
        maxSide = max(maxSide , length);
    }
    return maxSide;
}

double Polygon1::getMINSideLength()
{
    double minSide = 10000000;
    for (int i = 0; i < points.size(); ++i)
    {
        Point p1 = points[i];
        Point p2 = points[(i+1) % points.size()];
        double x1 = p1.getX() , y1 = p1.getY() , x2 = p2.getX() , y2 = p2.getY();

        double length = sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)) );
        minSide = min(minSide , length);
    }
    return minSide;
}

vector<Line> Polygon1::getLines()
{
    vector<Line> lines;

    for (int i = 0; i < points.size(); ++i)
    {
        Point p1 = points[i];
        Point p2 = points[(i+1) % points.size()];
        double x1 = p1.getX() , y1 = p1.getY() , x2 = p2.getX() , y2 = p2.getY();

        lines.emplace_back(x1 , y1, x2 , y2);
    }
    return lines;
}

Point Polygon1::minPoint() {
    Point p = points[0];

    for(auto &point : points)
    {
        p.setX(min(p.getX(),point.getX())) ;

        p.setY(min(p.getY(),point.getY())) ;
    }

//    Point p = points[0]; // Start with the first point
//
//    for (const auto &point : points) {
//        if (point.getX() < p.getX() || (point.getX() == p.getX() && point.getY() < p.getY())) {
//            p = point; // Update only if the whole point is smaller
//        }
//    }
    return p;
}

void Polygon1::transformPolygon(double dx, double dy)
{
    for(auto &point : points){
        point.setX(point.getX()+dx);
        point.setY(point.getY()+dy);
    }
}

Point Polygon1::maxPoint() {
    Point p = points[0];

    for(auto &point : points)
    {
        p.setX(max(p.getX(),point.getX())) ;

        p.setY(max(p.getY(),point.getY())) ;
    }
    return p;
}

void Polygon1::rotate(double angle)
{
    double angleRadians = angle * 3.14159 / 180;

    for(auto &point : points)
    {
        double x1New = 0 , y1New = 0 , x2New = 0 , y2New = 0;
        double x1 = point.getX() , y1 = point.getY();

        x1New = x1 * cos(angleRadians) - y1 * sin(angleRadians);
        y1New = x1 * sin(angleRadians) + y1 * cos(angleRadians);

        x1New = round( x1New * 10 ) / 10 ;
        y1New = round( y1New * 10 ) / 10 ;

        point.setX(x1New);
        point.setY  (y1New);
    }
}

void Polygon1::shiftX(double dx)
{

    for(auto &point : points)
    {
        point.setX(point.getX() + dx);
    }
}

void Polygon1::shiftY(double dy)
{

    for(auto &point : points)
    {
        point.setY  (point.getY() + dy);
    }
}

Point Polygon1::calculateCentroid()
{
    double signedArea = 0.0;
    double Cx = 0.0;
    double Cy = 0.0;
    int n = (int)points.size();

    for (int i = 0; i < n; ++i) {
        double x0 = points[i].getX();
        double y0 = points[i].getY();
        double x1 = points[(i + 1) % n].getX();
        double y1 = points[(i + 1) % n].getY();

        double a = x0 * y1 - x1 * y0;
        signedArea += a;
        Cx += (x0 + x1) * a;
        Cy += (y0 + y1) * a;
    }

    signedArea *= 0.5;
    Cx /= (6.0 * signedArea);
    Cy /= (6.0 * signedArea);

    return {Cx, Cy};
}

Line Polygon1::getCenterLine()
{
    int n = (int)points.size();
    if (n < 2) throw std::runtime_error("Polygon must have at least two points");

    Point centroid = calculateCentroid();
    double cx = centroid.getX(), cy = centroid.getY();

    double sxx = 0, sxy = 0, syy = 0;
    for (const auto& p : points) {
        double dx = p.getX() - cx;
        double dy = p.getY() - cy;
        sxx += dx * dx;
        sxy += dx * dy;
        syy += dy * dy;
    }

    double theta = 0.5 * atan2(2 * sxy, sxx - syy);
    double dx = cos(theta);
    double dy = sin(theta);

    double minProj = std::numeric_limits<double>::max();
    double maxProj = std::numeric_limits<double>::lowest();

    for (const auto& p : points) {
        double proj = (p.getX() - cx) * dx + (p.getY() - cy) * dy;
        minProj = std::min(minProj, proj);
        maxProj = std::max(maxProj, proj);
    }

    double x1 = cx + minProj * dx;
    double y1 = cy + minProj * dy;
    double x2 = cx + maxProj * dx;
    double y2 = cy + maxProj * dy;

    return {x1, y1, x2, y2};
}

vector<Point> Polygon1::scalePolygon(double scale)
{
    vector<Point> scaledVertices;
    Point centroid = calculateCentroid();

    for (auto& vertex : points)
    {
        Point scaledVertex;
        scaledVertex.setX( centroid.getX() + (vertex.getX() - centroid.getX()) * scale);
        scaledVertex.setY( centroid.getY() + (vertex.getY() - centroid.getY()) * scale);
        scaledVertices.push_back(scaledVertex);
    }

    return scaledVertices;
}

bool Polygon1::isDivisible() const {
    return divisible;
}

void Polygon1::setDivisible(bool _divisible) {
    Polygon1::divisible = _divisible;
}

int Polygon1::countAcuteAnglesLines() {
    int acuteLines = 0;

    vector<Line> polygonLines = getLines();

    for(auto &line : polygonLines)
    {
        double angle = line.getAngle();
        if (angle < 90 && angle > 0) acuteLines++;
    }
    return acuteLines;
}

const string &Polygon1::getId() const {
    return id;
}

void Polygon1::setId(const string &id) {
    Polygon1::id = id;
}

Line Polygon1::centroidLine() {
    Point centroid = calculateCentroid();
    vector<Line> lines = getLines();

    Point minPoint1 = minPoint();
    Point maxPoint1 = maxPoint();

    double dx = maxPoint1.getX() - minPoint1.getX();
    double dy = maxPoint1.getY() - minPoint1.getY();

    if (dy > dx)
    {
        return {centroid.getX() , centroid.getY() * -10 , centroid.getX() , centroid.getY() * 10};
    }
    else
    {
        return {centroid.getX() * -10, centroid.getY() , centroid.getX() * 10, centroid.getY() };
    }
}

vector<Line> Polygon1::computeCentroidPerpendiculars()
{
    Point centroid = calculateCentroid();

//    cout<<"centroid = "<<centroid.getX() <<" "<<centroid.getY()<<"\n";
    vector<Line> lines = getLines();
    vector<Line> perpendiculars;

    for(auto &line : lines)
    {
        double x1 = line.getX1() , x2 = line.getX2();
        double y1 = line.getY1() , y2 = line.getY2();
        double dx = x2 - x1;
        double dy = y2 - y1;


        double d = dx * dx + dy * dy;

        double t = ((centroid.getX() - x1) * dx + (centroid.getY()- y1) * dy) / d;

        Point p = { x1 + t * dx, y1 + t * dy};


        double x_intersect = p.getX();
        double y_intersect = p.getY();

        double minX = std::min(x1, x2), maxX = std::max(x1, x2);
        double minY = std::min(y1, y2), maxY = std::max(y1, y2);

        x_intersect = std::max(minX, std::min(x_intersect, maxX));
        y_intersect = std::max(minY, std::min(y_intersect, maxY));


        Line vertical (centroid.getX() , centroid.getY() , p.getX() , p.getY());
        Line vertical2 (centroid.getX() , centroid.getY() , x_intersect , y_intersect);

        dx = vertical2.getX2() - vertical2.getX1();
        dy = vertical2.getY2() - vertical2.getY1();
        vertical2.setX1(vertical2.getX1() - dx*1000);
        vertical2.setY1(vertical2.getY1() - dy*1000);
        perpendiculars.push_back(vertical2);
    }

    return perpendiculars;
}

Line Polygon1::getTallestLine() {
    vector<Line> lines = getLines();

    vector<pair<double , Line>> tallestLines;

    for(auto &line : lines)
    {
        tallestLines.emplace_back(line.getLength() , line);
    }
    std::sort(tallestLines.begin(), tallestLines.end() , greater<>());
    return tallestLines[0].second;
}

void Polygon1::addAdj(const string &_adjId) {
    adj.insert(_adjId);
}

const set<string> &Polygon1::getAdj() const {
    return adj;
}




