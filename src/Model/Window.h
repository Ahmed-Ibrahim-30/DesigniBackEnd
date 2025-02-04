//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_WINDOW_H
#define DESIGNI_WINDOW_H
#include "Line.h"

class Window{
private :
    Line windowLine;
public:
    Window(double x1,double y1,double x2,double y2){
        windowLine = Line( x1, y1, x2, y2);
    }
    Window(const Line &wLine)
    {
        windowLine = wLine;
        windowLine.setX1( round(windowLine.getX1() *10) / 10);
        windowLine.setY1( round(windowLine.getY1() *10) / 10);
        windowLine.setX2( round(windowLine.getX2() *10) / 10);
        windowLine.setY2( round(windowLine.getY2() *10) / 10);
    }
    Line getWindow()const {return windowLine;}
    void setWindow(const Line& line)
    {
        windowLine = line;
        windowLine.setX1( round(windowLine.getX1() *10) / 10);
        windowLine.setY1( round(windowLine.getY1() *10) / 10);
        windowLine.setX2( round(windowLine.getX2() *10) / 10);
        windowLine.setY2( round(windowLine.getY2() *10) / 10);
    }
    void mirrorX(){
        windowLine.setY1(- windowLine.getY1());
        windowLine.setY2(- windowLine.getY2());

        //swap Y1 by Y2
        double y1 = windowLine.getY1();
        windowLine.setY1( windowLine.getY2());
        windowLine.setY2( y1);
    }
    void mirrorY(){
        windowLine.setX1(- windowLine.getX1());
        windowLine.setX2(- windowLine.getX2());

        //swap X1 by X2
        double x1 = windowLine.getX1();
        windowLine.setX1( windowLine.getX2());
        windowLine.setX2( x1);
    }
};
#endif //DESIGNI_WINDOW_H
