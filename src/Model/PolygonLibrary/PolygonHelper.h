//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#ifndef DESIGNI_POLYGONHELPER_H
#define DESIGNI_POLYGONHELPER_H
#include "src/Model/Polygon.h"

/**
 * Static Class Contains all Polygon Required Function
 */
class PolygonHelper{
public:

    /**
     * Extend Line and return new Extended Line only not Line + extend Line
     * @param line
     * @param extendFromSecondPoint
     * @param scale factor
     * @return
     */
    static Line extendLine(const Line &line , bool extendFromSecondPoint = true , double scale = 100);

    /**
     * Get Intersection Point
     * @param line1
     * @param line2
     * @return
     */
    static Point getIntersectionPoint(const Line &line1 , const Line &line2);

    /**
     * @brief isPointInSegment
     * @param p
     * @param line
     * @return
     */
    static bool isPointInSegment(const Point &p, const Line &line);

    /**
     * @brief isPointInSegment
     * @param p
     * @param line
     * @return
     */
    static bool isPointOnLine(const Point& p, const Line& line);

    /**
     * Get Slope
     * @param x1
     * @param x2
     * @param y1
     * @param y2
     * @return
     */
    static double getSlope(double x1, double x2, double y1, double y2);

    /**
     * Get Second Point of Line Using Angle
     * @param x
     * @param y
     * @param angle
     * @return
     */
    static Point getSecondPointOfLineByAngle(double x , double y , double angle , double distance = 1000000);

    /**
     * isLineIntersectWithPolygon
     * @param pol
     * @param line
     * @return
     */
    static bool isLineIntersectWithPolygon(Polygon1 &pol , const Line &line);

    /**
     * Get Line Length
     * @param first
     * @param second
     * @return
     */
    static double getLineLength(const Point &first, const Point &second);

    /**
     *
     * @param outerLand
     * @param lands
     * @return
     */
    static set<int> getBoundaryLands(Polygon1 &outerLand ,vector<Polygon1> &lands) ;

    /**
     * Sort Polygons By Areas
     * @param polygons
     * @return
     */
    static vector<Polygon1> sortPolygonByArea(vector<Polygon1> &polygons);
};


#endif //DESIGNI_POLYGONHELPER_H
