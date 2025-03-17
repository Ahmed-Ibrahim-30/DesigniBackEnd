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

    /**
     * IS POLYGON MAY BE DIVIDED
     */
    bool divisible;

    /**
     * Polygon ID
     */
    string id;
public:
    const string &getId() const;

    void setId(const string &id);

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

    /**
     * Shift On X axis
     * @param dx
     */
    void shiftX(double dx);

    /**
     * Shift On Y Axis
     * @param dx
     */
    void shiftY(double dx);

    /**
     * @brief calculateCentroid
     * @return
     */
    Point calculateCentroid();

    Line centroidLine ();

    /**
     * Get Center Line
     * @return
     */
    Line getCenterLine();

    /**
     * @brief scalePolygon
     * @param scale
     * @return
     */
    vector<Point> scalePolygon(double scale);

    /**
     * Is Polygon valid to Divisible or Not
     * Is Green Area Or Not for lands
     * @param divisible
     */
    void setDivisible(bool divisible);

    /**
     * Count Acute Angles Lines
     * @return
     */
    int countAcuteAnglesLines();
};


#endif //DESIGNI_POLYGON_H
