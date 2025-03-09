//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#ifndef DESIGNI_BACKEND_DRAWSTREET_H
#define DESIGNI_BACKEND_DRAWSTREET_H
#include "src/Model/PolygonLibrary/PolygonHelper.h"

class DrawStreet {
private:
    vector<Line> centerLines;
    vector<vector<Line>> streets;

    vector<vector<Line>> drawTopStreets(const vector<Line> &polygonLines ,const vector<Line> &TopLine , double step = 40);
    vector<vector<Line>> drawBottomStreets(const vector<Line> &polygonLines ,const vector<Line> &bottomLine , double step = 40);
public:
    void drawStreets(Polygon1 &polygon1);
    const vector<Line> &getCenterLines() const;

    const vector<vector<Line>> &getStreets() const;
};


#endif //DESIGNI_BACKEND_DRAWSTREET_H
