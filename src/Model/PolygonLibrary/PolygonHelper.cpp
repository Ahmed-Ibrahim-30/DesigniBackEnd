//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#include "PolygonHelper.h"

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
    if (denominator == 0) {
        return {INT_MAX , INT_MAX}; // No intersection
    }

    // Calculate the intersection point
    double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
    double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

    // Check if the intersection point is within the bounds of both line segments
    if (px + 0.1 < std::min(x1, x2) || px > std::max(x1, x2)+ 0.1 ||
        px + 0.1 < std::min(x3, x4) || px > std::max(x3, x4)+ 0.1 ||
        py + 0.1 < std::min(y1, y2) || py > std::max(y1, y2)+ 0.1 ||
        py + 0.1 < std::min(y3, y4) || py > std::max(y3, y4)+ 0.1) {
        return {INT_MAX , INT_MAX}; // No intersection
    }

    return Point{px, py};
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

bool PolygonHelper::isPointOnLine(Point p, Line line) {
    // Calculate the cross product to check if the point is collinear with the line
    double crossProduct = (p.getY() - line.getY1()) * (line.getX2() - line.getX1()) - (p.getX() - line.getX1()) * (line.getY2() - line.getY1());

    // If the cross product is not zero, the point is not on the line
    if (abs(crossProduct) > 0.1) {
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


