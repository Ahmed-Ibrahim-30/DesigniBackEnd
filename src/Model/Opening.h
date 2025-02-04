//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_OPENING_H
#define DESIGNI_OPENING_H
#include "Line.h"

class Opening{
private :
    Line openingLine;
public:
    Opening(double x1,double y1,double x2,double y2){
        openingLine = Line( x1, y1, x2, y2);
    }
    Opening(const Line &dLine){
        openingLine = dLine;
    }
    Line getOpening() const {return openingLine;}
    void setOpening(const Line& line){openingLine = line;}

    void mirrorX(){
        openingLine.setY1(- openingLine.getY1());
        openingLine.setY2(- openingLine.getY2());

        //swap Y1 by Y2
        double y1 = openingLine.getY1();
        openingLine.setY1( openingLine.getY2());
        openingLine.setY2( y1);
    }
    void mirrorY(){
        openingLine.setX1(- openingLine.getX1());
        openingLine.setX2(- openingLine.getX2());

        //swap X1 by X2
        double x1 = openingLine.getX1();
        openingLine.setX1( openingLine.getX2());
        openingLine.setX2( x1);
    }
};
#endif //DESIGNI_OPENING_H
