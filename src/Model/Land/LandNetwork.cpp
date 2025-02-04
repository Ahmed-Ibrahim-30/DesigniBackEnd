//
// Created by ahmed Ibrahim on 13-Jan-25.
//

#include "LandNetwork.h"

bool LandNetwork::isPointInSegment(const Point &p, const Line &line)
{
    if(abs(line.getX1() - line.getX2()) <= 0.1 || abs(line.getY1() - line.getY2()) <= 0.1 )
    {
        Line line2 (min(line.getX1() , line.getX2()) , min(line.getY1() , line.getY2()) ,
                    max(line.getX1() , line.getX2()) , max(line.getY1() , line.getY2()));
        return (p.getX() <= line2.getX2() && p.getX() >= line2.getX1() &&
            p.getY() <= line2.getY2() && p.getY() >= line2.getY1()    );
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

    if(p.getX() < min(line.getX1() , line.getX2()) || p.getX() > max(line.getX1() , line.getX2())) return false;
    if(p.getY() < min(line.getY1() , line.getY2()) || p.getY() > max(line.getY1() , line.getY2())) return false;

    return true;
}

double LandNetwork::getSlope(double x1, double x2, double y1, double y2) {
    if(x2 == x1) return 0;

    return (y2-y1) / (x2 - x1);
}
