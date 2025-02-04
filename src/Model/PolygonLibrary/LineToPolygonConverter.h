//
// Created by ahmed Ibrahim on 23-Jan-25.
//

#ifndef DESIGNI_LINETOPOLYGONCONVERTER_H
#define DESIGNI_LINETOPOLYGONCONVERTER_H
#include "src/Model/Polygon.h"
#include "PolygonHelper.h"
/**
 * Class Responsible for convert some of lines to Polygons
 */
class LineToPolygonConverter {
private:
    /**
     * Input Lines
     */
    vector<Line> lines;

    void DFS(Point firstPoint,Point lastpoint,int index , set<int> &tst , vector<int> &ans, vector<vector<int>> &allAnswers);
    bool founded = false;

public:
    explicit LineToPolygonConverter(const vector<Line> &lines);

    [[nodiscard]] const vector<Line> &getLines() const;

    void setLines(const vector<Line> &lines);

    /**
     * Construct Polygons from vector of Lines
     * @return
     */
    vector<Polygon1> constructPolygons(Polygon1 &innerPolygon);

};


#endif //DESIGNI_LINETOPOLYGONCONVERTER_H
