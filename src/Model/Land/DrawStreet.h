//
// Created by ahmed Ibrahim on 06-Mar-25.
//

#ifndef DESIGNI_BACKEND_DRAWSTREET_H
#define DESIGNI_BACKEND_DRAWSTREET_H
#include "src/Model/PolygonLibrary/PolygonHelper.h"
#include "src/Model/Land/CityGrid.h"
#include "src/Model/Land/HomeLand.h"

class DrawStreet {
private:
    vector<Line> centerLines;

    vector<CityGrid> cities;

    double circleStreetWidth = 2.5;

    double divisionArea;
    double externalRoad;
    double centralRoad;
    double circularStreet;
    double landDepth;
    double streetCut;

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
    vector<Line> drawHomeBorders(Polygon1 &polygon1 , vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner  , vector<Line> &extensionsLin , vector<HomeLand> &homeLands,const Line &centerLine, int divisionIndex , int divisionsCount);
    vector<Line> drawInnerHomeBorders(Polygon1 &polygon1 , vector<Line> &streetsLinesOuter, vector<Line> &streetsLinesInner  , vector<Line> &extensionsLin , vector<HomeLand> &homeLands,const Line &centerLine, int divisionIndex , int divisionsCount);

    /**
     * Set Home inside each Land
     * @param lands
     * @param home
     */
    vector<Polygon1> homeSetter( vector<HomeLand> &lands , Polygon1 &home, Polygon1 &home2);
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

    void drawSide1Streets(const vector<Line> &polygonLines , const vector<Line> &centerL , vector<Line> &TopLine , double step  ,double startSpace,int divisions, const Line &innerCenterLine);


    vector<Line> SplitCenterLines(double startSpace ,double step1 , int divisions,const vector<Line> &centerLines2);

    /**
     * Find Best Center Lines
     * @param polygon
     * @param centerLineHeight
     * @return
     */
    vector<Line> buildCenterLines(Polygon1 &polygon ,double centerLineHeight, double startSpace , double step);


    /**
     * Return new Polygon
     * @param start
     * @param end
     * @param start2
     * @param end2
     * @param mainLandLines
     * @return
     */
    Polygon1 getHomePolygon(const Point &start, const Point &end, const Point &start2, const Point &end2,
                            const vector<Line> &mainLandLines, Polygon1 &pol, vector<Line> &bottomLines);

    int getMaxNumberOfDivisionsForLine(const Line &line , double initialStep , double initialStartStep, Polygon1 &boundaryPolygon , const Line &innerCenterLine, bool isTop);

    /**
     *
     * @param divisionsCount
     * @param centerLine
     * @return
     */
    double getAppropriateStep(int divisionsCount , const Line &centerLine,const Line &centerLineInner , Polygon1 &innerPolygon ,double startSearch = 40, bool isTop = true);
public:
    void drawStreets(Polygon1 &polygon1 ,double divisionArea, double externalRoad, double centralRoad, double circularStreet, double landDepth,
                     double streetCut);

    [[nodiscard]] const vector<Line> &getCenterLines() const;

    const vector<CityGrid> &getCities() const;


};


#endif //DESIGNI_BACKEND_DRAWSTREET_H
