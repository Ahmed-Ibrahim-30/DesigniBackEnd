//
// Created by ahmed Ibrahim on 24-Mar-25.
//

#include "HomeLand.h"

HomeLand::HomeLand(const vector<Point> &points, bool divisible_) : Polygon1(points, divisible_)
{

}

const vector<Line> &HomeLand::getCircleStreets() const {
    return circleStreets;
}

void HomeLand::setCircleStreets(const vector<Line> &circleStreets) {
    HomeLand::circleStreets = circleStreets;
}




