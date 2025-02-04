//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "RoomBuilder.h"

Room RoomBuilder::createRoom(const string &roomName , double roomArea, const vector<char> &view) {
    setRoomType(roomName);
    setRoomId( roomName);
    setArea(roomArea , roomType);
    setRequiredView(view);

    setDimensions(0.0 , 0.0 , 0.0 , 0.0);

    if(roomType == RoomType::Corridor)
    {
        setWidth(12);
        setHeight(1.5);
    }
    else
    {
        setWidth(0.0);
        setHeight(0.0);
    }

    return Room(this->roomType , this->roomId , this->area ,this->width , this->height , this->x1
            , this->y1 ,this->x2 , this->y2, this->requiredView);
}

Room RoomBuilder::createRoom(const string &roomName , double w, double h, const vector<char> &view) {
    setRoomType(roomName);
    setRoomId( roomName);
    setWidth(w);
    setHeight(h);
    setArea(width*height);
    setRequiredView(view);

    setDimensions(0.0 , 0.0 , width , height);

    return Room(this->roomType , this->roomId , this->area ,this->width , this->height , this->x1
     , this->y1 ,this->x2 , this->y2, this->requiredView);
}

Room RoomBuilder::createRoom(const string &roomName , double _x1, double _y1, double _x2, double _y2,
                             const vector<char> &view) {
    setRoomType(roomName);
    setRoomId( roomName);
    setDimensions(_x1 , _y1 ,_x2 , _y2);
    setWidth(x2 - x1);
    setHeight(y2 - y1);
    setArea(width*height);
    setRequiredView(view);

    setDimensions(x1 , y1 , x2 , y2);

    return Room(this->roomType , this->roomId , this->area ,this->width , this->height , this->x1
            , this->y1 ,this->x2 , this->y2, this->requiredView);
}


void RoomBuilder::setArea(double roomArea , RoomType roomType1) {
    if(roomArea == -1)
    {
        RoomBuilder::area = RoomDefault::getDefaultArea(roomType1);
    }
    else
    {
        RoomBuilder::area  = MathUtils::roundingToDecimal(roomArea);
    }
}

void RoomBuilder::setWidth(double w) {
    RoomBuilder::width  = MathUtils::roundingToDecimal(w);
}

void RoomBuilder::setHeight(double h) {
    RoomBuilder::height = MathUtils::roundingToDecimal(h);
}

void RoomBuilder::setRoomType(const string &roomName) {
    RoomBuilder::roomType = RoomTypeUtils::parseStringToRoomType(roomName);
}

void RoomBuilder::setRoomId( const string &roomName) {
    RoomBuilder::roomId = UniqueIdGenerator::getInstance()->generateId( roomName);
}

void RoomBuilder::setRequiredView(const vector<char> &reqView) {
    RoomBuilder::requiredView = reqView;
    //convert all character from lower to upper
    for(auto &ch : requiredView)
    {
        ch = toupper(ch);
    }
}

void RoomBuilder::setDimensions(double _x1, double _y1, double _x2, double _y2) {
    this->x1 = MathUtils::roundingToDecimal(_x1);
    this->y1 = MathUtils::roundingToDecimal(_y1);
    this->x2 = MathUtils::roundingToDecimal(_x2);
    this->y2 = MathUtils::roundingToDecimal(_y2);
}






