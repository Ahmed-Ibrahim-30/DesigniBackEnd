//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#ifndef DESIGNI_BACKEND_DRAWSTREET_H
#define DESIGNI_BACKEND_DRAWSTREET_H
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "src/Model/Land/CityGrid.h"

class DrawStreet {
private:
    vector<Line> centerLines;

    vector<CityGrid> cities;

    double startSpace = 35;

    Polygon1 mainLand;

    /**
     * Draw TOP BOTTOM Streets
     * @param polygonLines
     * @param TopLine
     * @param bottomLine
     * @param step
     * @return
     */
    vector<Line> drawExtensions(const vector<Line> &polygonLines ,const vector<Line> &topLines ,const Point &start , const Point &end , const Point &startUp , const Point &endUp , double step, bool isTop,const vector<Line> &centerL);
    vector<Line> drawHomeBorders(Polygon1 &polygon1 , vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner  , vector<Line> &extensionsLine, bool isTop);

    /**
     * Get Next Point after start Point with step variable
     * @param start
     * @param lineIndex
     * @param lines
     * @param step
     * @return
     */
    Point getNextPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step ,vector<Line> &connLines);
    Point getPrevPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step ,vector<Line> &connLines);

    vector<vector<Line>> drawTopStreets(const vector<Line> &polygonLines , const vector<Line> &centerL , const vector<Line> &TopLine , double step  ,int divisions);
    vector<vector<Line>> drawBottomStreets(const vector<Line> &polygonLines ,const vector<Line> &centerL,const vector<Line> &bottomLine , double step ,int divisions);


    vector<Line> SplitCenterLines(double startSpace ,double step1 , int divisions,const vector<Line> &centerLines2);

public:
    void drawStreets(Polygon1 &polygon1);

    [[nodiscard]] const vector<Line> &getCenterLines() const;

    const vector<CityGrid> &getCities() const;
};


#endif //DESIGNI_BACKEND_DRAWSTREET_H
