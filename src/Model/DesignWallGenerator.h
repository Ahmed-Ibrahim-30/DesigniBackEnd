#ifndef DESIGNWALLGENERATOR_H
#define DESIGNWALLGENERATOR_H
#include "Design.h"
#include "Polygon.h"
class DesignWallGenerator
{
private:
    static bool isBetween(double value, double bound1, double bound2);
    static double getIntersectionY(double x, const Point &p1, const Point &p2);
    static double getIntersectionX(double y, const Point &p1, const Point &p2);
public:
    static vector<Wall> generateWalls(const Design &design);

    static vector<Wall> constructRoof(const Polygon1 &polygon , double spacing = 10);

    static vector<Line> getDesignLines(const Design &design);
};

#endif // DESIGNWALLGENERATOR_H
