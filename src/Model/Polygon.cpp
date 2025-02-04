//
// Created by ahmed Ibrahim on 25-Dec-24.
//

#include "Polygon.h"

Polygon1::Polygon1(const vector<Point> &points_ , bool divisible_) : points(points_) , divisible(divisible_){

    if(points.size() > 1 && points[points.size()-1] == points[0])points.pop_back();

    if(points.size() >1)
    {
        // vector<Point> newP ;
        // newP.push_back(points[0]);
        // newP.push_back(points[1]);

        // for (int i = 2; i < points.size(); ++i)
        // {
        //     if(abs(points[i].getX() - points[i-1].getX()) <= 0.1 && abs(points[i-1].getX() - points[i-2].getX()) <= 0.1)
        //     {
        //         newP.pop_back();
        //     }
        //     else if(abs(points[i].getY() - points[i-1].getY()) <= 0.1 && abs(points[i-1].getY() - points[i-2].getY()) <= 0.1)
        //     {
        //         newP.pop_back();
        //     }
        //     newP.push_back(points[i]);
        // }
        // points = newP;
    }

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

void Polygon1::print() {
    // cout<<"Polygon -- .. \n";
    int n = (int)points.size();
    for (int i = 0; i < points.size(); ++i) {
        Point a1 = points[i];
        Point a2 = points[(i+1)%n];
        cout << "{ x1:" << a1.getX() << ", y1:" << a1.getY() << ", x2:" << a2.getX() << ", y2:" << a2.getY()<< "},\n";
    }
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
    Point centroid( 0 , 0 );
    for(auto &point : points)
    {
        centroid.setY(point.getY() + centroid.getY());
        centroid.setX(point.getX() + centroid.getX());
    }
    int n = (int)points.size();
    centroid.setY(centroid.getY() / n);
    centroid.setX(centroid.getX() / n);
    return centroid;
}

vector<Point> Polygon1::scalePolygon(double scale)
{
    vector<Point> scaledVertices;
    Point centroid = calculateCentroid();

    for (auto& vertex : points) {
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

void Polygon1::setDivisible(bool divisible) {
    Polygon1::divisible = divisible;
}
