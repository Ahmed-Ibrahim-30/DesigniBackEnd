//
// Created by ahmed Ibrahim on 24-Mar-25.
//

#ifndef DESIGNI_BACKEND_HOMELAND_H
#define DESIGNI_BACKEND_HOMELAND_H
#include "src/Model/Polygon.h"

class HomeLand : public Polygon1{
private:
    vector<Line> circleStreets;
public:
    explicit HomeLand(const vector<Point> &points , bool divisible_ = true);

    const vector<Line> &getCircleStreets() const;

    void setCircleStreets(const vector<Line> &circleStreets);

};


#endif //DESIGNI_BACKEND_HOMELAND_H
