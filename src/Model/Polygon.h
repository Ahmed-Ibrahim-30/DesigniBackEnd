//
// Created by ahmed Ibrahim on 25-Dec-24.
//

#ifndef DESIGNI_POLYGON_H
#define DESIGNI_POLYGON_H
#include "Point.h"
#include "Line.h"
class Polygon1 {
private:
    /**
     * Polygon Points
     */
    vector<Point> points;
    bool divisible;

public:
    const vector<Point> &getPoints() const;

    void setPoints(const vector<Point> &points);

    explicit Polygon1(const vector<Point> &points , bool divisible_ = true);
    explicit Polygon1() = default;

    /**
     * @brief getArea
     * @return
     */
    double getArea();

    vector<Point> getOutsidePoints();

    /**
     * Print Polygon Details
     */
    void print();

    bool isDivisible() const;

    /**
     * Get difference between MIN and Max Side Length
     * @return
     */
    double getDifferenceBetweenMINMAXSideLength();

    double getMAXSideLength();

    double getMINSideLength();

    vector<Line> getLines();

    Point minPoint();
    Point maxPoint();

    void transformPolygon(double dx,double dy);

    void rotate(double angle);

    void shiftX(double dx);
    void shiftY(double dx);

    /**
     * @brief calculateCentroid
     * @return
     */
    Point calculateCentroid();

    /**
     * @brief scalePolygon
     * @param scale
     * @return
     */
    vector<Point> scalePolygon(double scale);

    void setDivisible(bool divisible);
};


#endif //DESIGNI_POLYGON_H
