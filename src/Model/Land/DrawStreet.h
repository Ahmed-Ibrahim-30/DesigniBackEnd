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

    double startSpace = 20;
    double circleStreetWidth = 2.5;
    double step = 40;

    /**
     * Main Land
     */
    Polygon1 mainLand;

    /**
     * Draw TOP BOTTOM Streets
     * @param polygonLines
     * @param TopLine
     * @param bottomLine
     * @param step
     * @return
     */
    vector<Line> drawExtensions(const vector<Line> &polygonLines ,const Line &topLines ,const Point &start , const Point &end , const Point &startUp , const Point &endUp , double step,const Line &centerL , int divisionIndex , int divisionsCount);
    vector<Line> drawHomeBorders(Polygon1 &polygon1 , vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner  , vector<Line> &extensionsLin , vector<Polygon1> &homeLands,const Line &centerLine);

    /**
     * Set Home inside each Land
     * @param lands
     * @param home
     */
    vector<Polygon1> homeSetter( vector<Polygon1> &lands , Polygon1 &home);
    /**
     * Get Next Point after start Point with step variable
     * @param start
     * @param lineIndex
     * @param lines
     * @param step
     * @return
     */
    Point getNextPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step );
    Point getNextPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step , vector<Line> &bottomLines);
    Point getPrevPoint(const Point &start, int &lineIndex ,const vector<Line> &lines, double step);

    void drawSide1Streets(const vector<Line> &polygonLines , const vector<Line> &centerL , const vector<Line> &TopLine , double step  ,int divisions);


    vector<Line> SplitCenterLines(double startSpace ,double step1 , int divisions,const vector<Line> &centerLines2);

    /**
     * Find Best Center Lines
     * @param polygon
     * @param centerLineHeight
     * @return
     */
    vector<Line> buildCenterLines(Polygon1 &polygon ,double centerLineHeight);
public:
    void drawStreets(Polygon1 &polygon1);

    [[nodiscard]] const vector<Line> &getCenterLines() const;

    const vector<CityGrid> &getCities() const;
};


#endif //DESIGNI_BACKEND_DRAWSTREET_H
