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

    /**
     * Polygons Adj
     */
    set<string> adj;
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

    /**
     * Get Tallest Line
     * @return
     */
    Line getTallestLine();

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

    /**
     *
     * @return
     */
    vector<Line> computeCentroidPerpendiculars();

    /**
     * Line pass to Centroid Point
     * @return
     */
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

    const string &getId() const;

    void setId(const string &id);

    /**
     *
     * @param adj
     * @return
     */
    void addAdj(const string &adj);

    const set<string> &getAdj() const;
};


#endif //DESIGNI_POLYGON_H
