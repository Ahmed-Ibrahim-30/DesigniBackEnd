//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_LINE_H
#define DESIGNI_LINE_H
#include "Point.h"

class Line {
private:
    double x1, x2, y1, y2,z1,z2;
    Point start,end;
    double centerX1, centerX2, centerY1, centerY2;
    char dir;
    string id;//rectangle name
    string roomName;
    string id2;//window or door
    int sideNum;//1 2 3 4
    char face;
    double x1_Corr,x2_Corr,y1_Corr,y2_Corr;
public:
    const Point &getStart() const;

    const Point &getAnEnd() const;

    void scale(double unit = 100 ){
        setX1(x1 * unit);
        setY1(y1 * unit);
        setX2(x2 * unit);
        setY2(y2 * unit);
    }
    void moveLine(double xChanged, double yChanged){
        setX1( getX1() + xChanged);
        setY1( getY1() + yChanged);
        setX2( getX2() + xChanged);
        setY2( getY2() + yChanged);
    }


private:
    string color;
    double startAngle,endAngle;
    string sideRight,sideLeft;
    Point first,second;
    /**
     * external or internal
     */
    string relationToRectangles;

public:
    Line(){
        x1=0,y1=0,x2=0,y2=0;
        color="white";
        startAngle=0;
        endAngle=0;
    }
    Line(double x1, double y1, double x2, double y2);
    Line(double x1, double x2, double y1, double y2,double z1,double z2);
    [[nodiscard]] const string &getId() const;

    [[nodiscard]] double getCenterX1() const;
    void setDir(char d);
    void setDir();

    void setCenterX1(double centerX1);

    [[nodiscard]] double getCenterX2() const;

    void setCenterX2(double centerX2);

    [[nodiscard]] double getCenterY1() const;

    void setCenterY1(double centerY1);

    [[nodiscard]] double getCenterY2() const;

    void setCenterY2(double centerY2);

    void setId(const string &id);
    [[nodiscard]] const Point &getFirst() const;

    void setFirst(const Point &first);

    [[nodiscard]] const Point &getSecond() const;

    void setSecond(const Point &second);

    [[nodiscard]] const string &getSideRight() const;

    void setSideRight(const string &sideRight);

    [[nodiscard]] const string &getSideLeft() const;

    void setSideLeft(const string &sideLeft);

    bool operator<(const Line &l) const ;
    bool operator==(const Line &l) const ;
    [[nodiscard]] double getX1() const;

    void setX1(double x1);

    [[nodiscard]] double getX2() const;

    void setX2(double x2);

    [[nodiscard]] double getY1() const;

    void setY1(double y1);

    [[nodiscard]] double getY2() const;

    void setY2(double y2);
    [[nodiscard]] char getDir() const;
    void print() const ;

    [[nodiscard]] int getSideNum() const;

    [[nodiscard]] double getZ1() const;

    void setZ1(double z1);

    [[nodiscard]] double getZ2() const;

    void setZ2(double z2);

    void setSideNum(int sideNum);

    [[nodiscard]] const string &getColor() const;

    void setColor(const string &color);

    [[nodiscard]] double getStartAngle() const;

    void setStartAngle(double startAngle);

    [[nodiscard]] double getEndAngle() const;

    void setEndAngle(double endAngle);

    [[nodiscard]] const string &getId2() const;

    void setId2(const string &id2);
    [[nodiscard]] const string &getRoomName() const;

    void setRoomName(const string &roomName);

    [[nodiscard]] const string &getRelationToRectangles() const;

    void setRelationToRectangles(const string &relationToRectangles);

    [[nodiscard]] char getFace() const;

    void setFace(char face);

    double getX1Corr() const{return x1_Corr;}

    double getX2Corr() const{return x2_Corr;}

    double getY1Corr() const{return y1_Corr;}

    double getY2Corr() const{return y2_Corr;}

    void printJsonFormat();

    bool operator!=(const Line &p) const {
        return !(*this == p);
    }

};


#endif //DESIGNI_LINE_H
