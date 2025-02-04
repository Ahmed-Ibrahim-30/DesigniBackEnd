//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_DOOR_H
#define DESIGNI_DOOR_H
#include "Line.h"

class Door{
private :
    Line doorLine;
public:
    Door(double x1,double y1,double x2,double y2){
        doorLine = Line( x1, y1, x2, y2);
    }
    Door(const Line &dLine){
        doorLine = dLine;
    }
    const Line &getDoor() const {return doorLine;}
    void setDoor(const Line& line){doorLine = line;}

    void mirrorX(){
        doorLine.setY1(- doorLine.getY1());
        doorLine.setY2(- doorLine.getY2());

        //swap Y1 by Y2
        double y1 = doorLine.getY1();
        doorLine.setY1( doorLine.getY2());
        doorLine.setY2( y1);
    }
    void mirrorY(){
        doorLine.setX1(- doorLine.getX1());
        doorLine.setX2(- doorLine.getX2());

        //swap X1 by X2
        double x1 = doorLine.getX1();
        doorLine.setX1( doorLine.getX2());
        doorLine.setX2( x1);
    }
};
#endif //DESIGNI_DOOR_H
