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
    static bool isPointOnLine(Point p, Line line);

    /**
     * Get Slope
     * @param x1
     * @param x2
     * @param y1
     * @param y2
     * @return
     */
    static double getSlope(double x1, double x2, double y1, double y2);
};


#endif //DESIGNI_POLYGONHELPER_H
