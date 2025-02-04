//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "Room.h"

Room::Room(RoomType roomType , const string &roomId , double area,double width, double height, double x1, double y1, double x2, double y2,
           const vector<char> &requiredView) : area(area), x1(x1), y1(y1), x2(x2), y2(y2),
                                               width(width), height(height), roomType(roomType),
                                               roomId(roomId), requiredView(requiredView) {

}

void Room::scaleRoom(double unit)
{
    setX1(getX1() * unit);
    setY1(getY1() * unit);
    setX2(getX2() * unit);
    setY2(getY2() * unit);

    width = getX2() - getX1();
    height = getY2() - getY1();

    for(auto &window: windows )
    {
        double x1L, y1L, x2L, y2L;
        Line line=window.getWindow();
        x1L = line.getX1();
        x2L = line.getX2();
        y1L = line.getY1();
        y2L = line.getY2();
        line.setX1(x1L * unit);
        line.setY1(y1L * unit);
        line.setX2(x2L * unit);
        line.setY2(y2L * unit);
        window= Window(line);
    }

    for(auto &door: doors )
    {
        double x1L, y1L, x2L, y2L;
        Line line=door.getDoor();
        x1L = line.getX1();
        x2L = line.getX2();
        y1L = line.getY1();
        y2L = line.getY2();
        line.setX1(x1L * unit);
        line.setY1(y1L * unit);
        line.setX2(x2L * unit);
        line.setY2(y2L * unit);
        door= Door(line);
    }

    for(auto &opening: openings )
    {
        double x1L, y1L, x2L, y2L;
        Line line=opening.getOpening();
        x1L = line.getX1();
        x2L = line.getX2();
        y1L = line.getY1();
        y2L = line.getY2();
        line.setX1(x1L * unit);
        line.setY1(y1L * unit);
        line.setX2(x2L * unit);
        line.setY2(y2L * unit);
        opening= Opening(line);
    }
}

void Room::moveRoom(double xChanged, double yChanged)
{
    setX1(getX1() + xChanged);
    setY1(getY1() + yChanged);
    setX2(getX2() + xChanged);
    setY2(getY2() + yChanged);

    for(auto &window:windows)
    {
        Line line = window.getWindow();
        line.setX1( line.getX1() + xChanged);
        line.setY1( line.getY1() + yChanged);
        line.setX2( line.getX2() + xChanged);
        line.setY2( line.getY2() + yChanged);
        window = Window(line);
    }
    for(auto &door:doors)
    {
        Line line = door.getDoor();
        line.setX1( line.getX1() + xChanged);
        line.setY1( line.getY1() + yChanged);
        line.setX2( line.getX2() + xChanged);
        line.setY2( line.getY2() + yChanged);
        door = Door(line);
    }

    for(auto &opening:openings)
    {
        Line line = opening.getOpening();
        line.setX1( line.getX1() + xChanged);
        line.setY1( line.getY1() + yChanged);
        line.setX2( line.getX2() + xChanged);
        line.setY2( line.getY2() + yChanged);
        opening = Opening(line);
    }
}

double Room::getArea() const {
    return area;
}

double Room::getX1() const {
    return x1;
}

double Room::getY1() const {
    return y1;
}

double Room::getX2() const {
    return x2;
}

double Room::getY2() const {
    return y2;
}

double Room::getWidth() const {
    return width;
}

double Room::getHeight() const {
    return height;
}

RoomType Room::getRoomType() const {
    return roomType;
}

const string &Room::getRoomId() const {
    return roomId;
}

const vector<char> &Room::getRequiredView() const {
    return requiredView;
}

void Room::changeArea(double newArea) {
    area = newArea;

    if(newArea == -1)
    {
        area = RoomDefault::getDefaultArea(roomType);
    }
    else
    {
        area  = MathUtils::roundingToDecimal(newArea);
    }
}

void Room::changeId(const string &newId) {
    roomId = UniqueIdGenerator::getInstance()->generateId( newId);
    updateRoomType(roomId);
}

void Room::updateRoomType(const string &roomName) {
    roomType = RoomTypeUtils::parseStringToRoomType(roomName);
}

void Room::printDetails() const{
    cout<<"Room : "<<roomId <<" Room Type : ["<<RoomTypeUtils::parseRoomTypeToString(roomType)<<
    "] - Area = "<<area <<" - Width = "<<width << " - Height = "<<height <<"\nDimensions = "<<
    x1 <<" "<<y1 <<" "<<x2 <<" "<<y2<<"\n";
    cout<<"requiredView = ";
    for(const auto &view : requiredView)cout<<view<<" ";

    cout<<"Windows \n";
    for(const auto &w : windows)cout<<w.getWindow().getX1() <<" " << w.getWindow().getY1()<<" "<<w.getWindow().getX2()<<" "<<w.getWindow().getY2()<<"\n";

    cout<<"Door \n";
    for(const auto &w : doors)cout<<w.getDoor().getX1() <<" " << w.getDoor().getY1()<<" "<<w.getDoor().getX2()<<" "<<w.getDoor().getY2()<<"\n";

    cout<<"Openning \n";
    for(const auto &w : openings)cout<<w.getOpening().getX1() <<" " << w.getOpening().getY1()<<" "<<w.getOpening().getX2()<<" "<<w.getOpening().getY2()<<"\n";

    cout<<"\n";
}

void Room::setX1(double _x1) {
    Room::x1 = MathUtils::roundingToDecimal(_x1);
}

void Room::setY1(double _y1) {
    Room::y1 = MathUtils::roundingToDecimal(_y1);
}

void Room::setX2(double _x2) {
    Room::x2 = MathUtils::roundingToDecimal(_x2);
    setWidth (getX2() - getX1());
}

void Room::setY2(double _y2) {
    Room::y2 = MathUtils::roundingToDecimal(_y2);
    setHeight (getY2() - getY1());
}

void Room::setWidth(double _width)
{
    Room::width = MathUtils::roundingToDecimal(_width);
}

void Room::setHeight(double _height)
{
    Room::height = MathUtils::roundingToDecimal(_height);
}

void Room::setRoomDimensions(double _width , double _height , double _x1 , double _y1)
{
    setWidth(_width);
    setHeight(_height);
    setX1(_x1);
    setY1(_y1);
    setX2(getX1() + getWidth());
    setY2(getY1() + getHeight());
}

void Room::rotate(double angle)
{
    double angleRadians = angle * 3.14159 / 180;

    double x1New = 0 , y1New = 0 , x2New = 0 , y2New = 0;

    x1New = x1 * cos(angleRadians) - y1 * sin(angleRadians);
    y1New = x1 * sin(angleRadians) + y1 * cos(angleRadians);
    x2New = x2 * cos(angleRadians) - y2 * sin(angleRadians);
    y2New = x2 * sin(angleRadians) + y2 * cos(angleRadians);

    x1New = round( x1New * 10 ) / 10 ;
    y1New = round( y1New * 10 ) / 10 ;
    x2New = round( x2New * 10 ) / 10 ;
    y2New = round( y2New * 10 ) / 10 ;

    x1 = x1New;
    y1 = y1New;
    x2 = x2New;
    y2 = y2New;


     for(auto &window:windows){
         Line line = window.getWindow();

         x1New = line.getX1() * cos(angleRadians) - line.getY1() * sin(angleRadians);
         y1New = line.getX1() * sin(angleRadians) + line.getY1() * cos(angleRadians);
         x2New = line.getX2() * cos(angleRadians) - line.getY2() * sin(angleRadians);
         y2New = line.getX2() * sin(angleRadians) + line.getY2() * cos(angleRadians);

         line.setX1(round(x1New*10)/10);
         line.setY1(round(y1New*10)/10);
         line.setX2(round(x2New*10)/10);
         line.setY2(round(y2New*10)/10);
         window = Window(line);
     }

     for(auto &door:doors){
         Line line = door.getDoor();

         x1New = line.getX1() * cos(angleRadians) - line.getY1() * sin(angleRadians);
         y1New = line.getX1() * sin(angleRadians) + line.getY1() * cos(angleRadians);
         x2New = line.getX2() * cos(angleRadians) - line.getY2() * sin(angleRadians);
         y2New = line.getX2() * sin(angleRadians) + line.getY2() * cos(angleRadians);

         line.setX1(round(x1New*10)/10);
         line.setY1(round(y1New*10)/10);
         line.setX2(round(x2New*10)/10);
         line.setY2(round(y2New*10)/10);
         door = Door(line);
     }

     for(auto &opening:openings){
         Line line = opening.getOpening();

         x1New = line.getX1() * cos(angleRadians) - line.getY1() * sin(angleRadians);
         y1New = line.getX1() * sin(angleRadians) + line.getY1() * cos(angleRadians);
         x2New = line.getX2() * cos(angleRadians) - line.getY2() * sin(angleRadians);
         y2New = line.getX2() * sin(angleRadians) + line.getY2() * cos(angleRadians);

         line.setX1(round(x1New*10)/10);
         line.setY1(round(y1New*10)/10);
         line.setX2(round(x2New*10)/10);
         line.setY2(round(y2New*10)/10);
         opening = Opening(line);
     }
}

void Room::mirrorX(){
    setY1(-getY1());
    setY2(-getY2());
    swap(y1 , y2);

    for(auto &window:windows)
    {
        window.mirrorX();
    }

    for(auto &door:doors)
    {
        door.mirrorX();
    }

    for(auto &op:openings)
    {
        op.mirrorX();
    }
}

void Room::mirrorY(){
    setX1(-getX1());
    setX2(-getX2());
    swap(x1 , x2);

    for(auto &window:windows)
    {
        window.mirrorY();
    }

    for(auto &door:doors)
    {
        door.mirrorY();
    }

    for(auto &op:openings)
    {
        op.mirrorY();
    }
}

void Room::addWindow(double x1, double y1, double x2, double y2) {
    windows.emplace_back(x1 , y1 , x2 , y2);
}

void Room::addDoor(double x1, double y1, double x2, double y2)
{
    doors.emplace_back(x1 , y1 , x2 , y2);
}

void Room::addOpening(double x1, double y1, double x2, double y2)
{
    openings.emplace_back(x1 , y1 , x2 , y2);
}

void Room::shiftX(double dx)
{
    setX1(getX1()+dx);
    setX2(getX2()+dx);

    for(auto &window:windows)
    {
        Line line = window.getWindow();
        line.setX1( line.getX1() + dx);
        line.setX2( line.getX2() + dx);
        window = Window(line);
    }
    for(auto &door:doors)
    {
        Line line = door.getDoor();
        line.setX1( line.getX1() + dx);
        line.setX2( line.getX2() + dx);
        door = Door(line);
    }

    for(auto &opening:openings)
    {
        Line line = opening.getOpening();
        line.setX1( line.getX1() + dx);
        line.setX2( line.getX2() + dx);
        opening = Opening(line);
    }
}

void Room::shiftY(double dy)
{
    setY1(getY1()+dy);
    setY2(getY2()+dy);

    for(auto &window:windows)
    {
        Line line = window.getWindow();
        line.setY1( line.getY1() + dy);
        line.setY2( line.getY2() + dy);
        window = Window(line);
    }

    for(auto &door:doors)
    {
        Line line = door.getDoor();
        line.setY1( line.getY1() + dy);
        line.setY2( line.getY2() + dy);
        door = Door(line);
    }

    for(auto &opening:openings)
    {
        Line line = opening.getOpening();
        line.setY1( line.getY1() + dy);
        line.setY2( line.getY2() + dy);
        opening = Opening(line);
    }
}

bool Room::isPointInsideRoom(const Point &p) const {
    return (p.getX() > x1 && p.getX() < x2 && p.getY() > y1 && p.getY()<y2);
}

vector<Window> &Room::getWindows() {
    return windows;
}

vector<Door> &Room::getDoors() {
    return doors;
}

vector<Opening> &Room::getOpenings() {
    return openings;
}

vector<Window> Room::getWindows1() const {
    return windows;
}

const vector<string> &Room::getRoomsBesides() const {
    return roomsBesides;
}

void Room::setRoomsBesides(const vector<string> &roomsBesides) {
    Room::roomsBesides = roomsBesides;
}

const string &Room::getSurfaceSide() const {
    return surfaceSide;
}

void Room::setSurfaceSide(const string &surfaceSide) {
    Room::surfaceSide = surfaceSide;
}

double Room::getZ1() const {
    return z1;
}

void Room::setZ1(double _z1) {
    Room::z1 =  MathUtils::roundingToDecimal(_z1);
}

double Room::getZ2() const {
    return z2;
}

void Room::setZ2(double _z2) {
    Room::z2 =  MathUtils::roundingToDecimal(_z2);
}









