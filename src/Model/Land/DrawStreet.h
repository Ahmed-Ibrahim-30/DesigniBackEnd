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
    vector<vector<Line>> roadExtension;

    vector<vector<Line>> homeBorder;

    /**
     * Get Next Point after start Point with step variable
     * @param start
     * @param lineIndex
     * @param lines
     * @param step
     * @return
     */
    Point getNextPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step ,vector<Line> &connLines);

    vector<vector<Line>> drawTopStreets(const vector<Line> &polygonLines ,const vector<Line> &TopLine , double step = 40);
    vector<vector<Line>> drawBottomStreets(const vector<Line> &polygonLines ,const vector<Line> &bottomLine , double step = 40);


public:
    void drawStreets(Polygon1 &polygon1);


    [[nodiscard]] const vector<vector<Line>> &getRoadExtension() const;

    [[nodiscard]] const vector<vector<Line>> &getHomeBorder() const;

    [[nodiscard]] const vector<Line> &getCenterLines() const;

    [[nodiscard]] const vector<vector<Line>> &getStreets() const;
};


#endif //DESIGNI_BACKEND_DRAWSTREET_H
