//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#include "Line.h"
Line::Line(double x1, double y1, double x2, double y2) {

    setX1(x1);
    setX2(x2);
    setY1(y1);
    setY2(y2);

    if (x1==x2)dir='v';
    else dir='h';
    id="0";
    id2="0";
    sideNum=0;
    color="#ffffff";
    startAngle=0,endAngle=0;
    z1=0,z2=0;
    face='0';
    start.setX(x1);
    start.setY(y1);
    end.setX(x2);
    end.setY(y2);
}
Line::Line(double x1, double x2, double y1, double y2,double z1,double z2){

    setX1(x1);
    setX2(x2);
    setY1(y1);
    setY2(y2);
    setZ1(z1);
    setZ2(z2);

    if (x1==x2)dir='v';
    else dir='h';
    id="0";
    id2="0";
    sideNum=0;
    color="#ffffff";
    startAngle=0,endAngle=0;
    face='0';
    start.setX(x1);
    start.setY(y1);
    end.setX(x2);
    end.setY(y2);
}
double Line::getX1() const {
    return x1;
}
void Line::setDir(char d){
    dir=d;
}
void Line::setDir(){
    if (x1==x2)dir='v';
    else dir='h';
}

void Line::setX1(double _x1) {
    Line::x1 = MathUtils::roundingToDecimal(_x1);
}

double Line::getX2() const {
    return x2;
}

void Line::setX2(double _x2) {
    Line::x2 = MathUtils::roundingToDecimal(_x2);
}

double Line::getY1() const {
    return y1;
}

void Line::setY1(double _y1) {
    Line::y1 = MathUtils::roundingToDecimal(_y1);
}

double Line::getY2() const {
    return y2;
}

void Line::setY2(double _y2) {
    Line::y2 = MathUtils::roundingToDecimal(_y2);
}

void Line::print() const {
    std::cout<<id<<" "<<id2<<" "<< x1<<" "<<y1<<" "<<x2<<" "<<y2<<"\n";
}

bool Line::operator<(const Line &l) const {
    if (x1 != l.x1)return x1 < l.x1;
    if (y1 != l.y1)return y1 < l.y1;
    if (x2 != l.x2)return x2 < l.x2;
    return y2 < l.y2;
}

const string &Line::getSideRight() const {
    return sideRight;
}

void Line::setSideRight(const string &sideRight) {
    Line::sideRight = sideRight;
}

const string &Line::getSideLeft() const {
    return sideLeft;
}

void Line::setSideLeft(const string &sideLeft) {
    Line::sideLeft = sideLeft;
}

char Line::getDir() const {
    return dir;
}

const Point &Line::getFirst() const {
    return first;
}

void Line::setFirst(const Point &_first) {
    first.setX(_first.getX());
    first.setY(_first.getY());
    first.setId(_first.getId());
}

const Point &Line::getSecond() const {
    return second;
}

void Line::setSecond(const Point &_second) {
    second.setX(_second.getX());
    second.setY(_second.getY());
    second.setId(_second.getId());
}

bool Line::operator==(const Line &l) const {
    return abs(x1 - l.x1) <= 0.1 && abs (y1 - l.y1) <= 0.1 &&
            abs(x2 - l.x2) <= 0.1 && abs (y2 - l.y2) <= 0.1;
}

const string &Line::getId() const {
    return id;
}

void Line::setId(const string &id) {
    Line::id = id;
}

const string &Line::getId2() const {
    return id2;
}

void Line::setId2(const string &id2) {
    Line::id2 = id2;
}

int Line::getSideNum() const {
    return sideNum;
}

void Line::setSideNum(int _sideNum) {
    Line::sideNum = _sideNum;
}

const string &Line::getColor() const {
    return color;
}

void Line::setColor(const string &color) {
    Line::color = color;
}

double Line::getStartAngle() const {
    return startAngle;
}

void Line::setStartAngle(double startAngle) {
    Line::startAngle = startAngle;
}

double Line::getEndAngle() const {
    return endAngle;
}

void Line::setEndAngle(double endAngle) {
    Line::endAngle = endAngle;
}

double Line::getZ1() const {
    return z1;
}

void Line::setZ1(double z1) {
    Line::z1 = z1;
}

double Line::getZ2() const {
    return z2;
}

void Line::setZ2(double z2) {
    Line::z2 = z2;
}

double Line::getCenterX1() const {
    return centerX1;
}

void Line::setCenterX1(double centerX1) {
    Line::centerX1 = centerX1;
}

double Line::getCenterX2() const {
    return centerX2;
}

void Line::setCenterX2(double centerX2) {
    Line::centerX2 = centerX2;
}

double Line::getCenterY1() const {
    return centerY1;
}

void Line::setCenterY1(double centerY1) {
    Line::centerY1 = centerY1;
}

double Line::getCenterY2() const {
    return centerY2;
}

void Line::setCenterY2(double centerY2) {
    Line::centerY2 = centerY2;
}

const string &Line::getRoomName() const {
    return roomName;
}

void Line::setRoomName(const string &roomName) {
    Line::roomName = roomName;
}

const string &Line::getRelationToRectangles() const {
    return relationToRectangles;
}

void Line::setRelationToRectangles(const string &relationToRectangles) {
    Line::relationToRectangles = relationToRectangles;
}

char Line::getFace() const {
    return face;
}

void Line::setFace(char face) {
    Line::face = face;
}

const Point &Line::getStart() const {
    return start;
}

const Point &Line::getAnEnd() const {
    return end;
}

void Line::printJsonFormat() {
    cout << "{ x1:" << x1 << ", y1:" << y1 << ", x2:" << x2 << ", y2:" << y2<< "},\n";
}
