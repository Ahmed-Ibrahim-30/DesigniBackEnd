//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "Design.h"
#include "DesignWallGenerator.h"

Design::Design()
{
    setSpacesCount(0);
    setBedroomsCount(0);
    setRoomsCount(0);
    setArea(0);
    initializeMainDesign();
}

void Design::addRoom(const Room &room) {
    const string& id = room.getRoomId();

    if(mapIdToIndex.count(id))
    {
        LOG(LogLevel::Warning , "Room : " + id + " Already added");
        return;
    }

    int index = (int)rooms.size();
    rooms.push_back(room);
    mapIdToIndex [id] = index;

    if(room.getRoomType() != RoomType::Corridor)
    {
        setSpacesCount(spacesCount + 1);
    }
}

void Design::setRooms(const vector<Room> &_rooms) {
    clearRoom();
    for(auto &room : _rooms)
    {
        addRoom(room);
    }
}

void Design::deleteRoom(const Room &room) {
    const string& id = room.getRoomId();

    if(mapIdToIndex.count(id) == false)
    {
        LOG(LogLevel::Warning , "Room : " + id + " doesnt exist");
        return;
    }

    int roomIndex = mapIdToIndex[id];
    rooms.erase(rooms.begin() + roomIndex);
    mapIdToIndex.erase(id);
    connections.erase(id);
    for(auto &roomC : connections)
    {
        if(roomC.second.count(id))roomC.second.erase(id);
    }
    setSpacesCount(spacesCount - 1);
}

void Design::deleteRoom(const string &roomId) {

    if(mapIdToIndex.count(roomId) == false)
    {
        LOG(LogLevel::Warning , "Room : " + roomId + " doesnt exist");
        return;
    }

    int roomIndex = mapIdToIndex[roomId];
    rooms.erase(rooms.begin() + roomIndex);
    mapIdToIndex.erase(roomId);
    connections.erase(roomId);
    for(auto &roomC : connections)
    {
        if(roomC.second.count(roomId))roomC.second.erase(roomId);
    }
    setSpacesCount(spacesCount - 1);
    LOG(LogLevel::Debug , "Room : " + roomId + " deleted successfully");
}

const map<string, set<string>> &Design::getConnections() const {
    return connections;
}

void Design::changeRoomArea(const string &id, double newArea) {
    if(mapIdToIndex.count(id) == false)
    {
        LOG(LogLevel::Warning , "Room : " + id + " doesnt exist");
        return;
    }

    int roomIndex = mapIdToIndex[id];
    rooms[roomIndex].changeArea(newArea);
}

void Design::changeRoomName(const string &oldId, const string &newName) {
    if(mapIdToIndex.count(oldId) == false)
    {
        LOG(LogLevel::Warning , "Room : " + oldId + " doesnt exist");
        return;
    }

    int roomIndex = mapIdToIndex[oldId];
    rooms[roomIndex].changeId(newName);
}

void Design::changeRoomName(RoomType roomType, const string &newName) {
    string oldId = "";

    for(auto &room : rooms)
    {
        if(room.getRoomType() == roomType)
        {
            oldId = room.getRoomId();
            break;
        }
    }

    if(mapIdToIndex.count(oldId) == false)
    {
        LOG(LogLevel::Warning , "Room : " + RoomTypeUtils::parseRoomTypeToString(roomType) + " doesnt exist");
        return;
    }

    int roomIndex = mapIdToIndex[oldId];
    rooms[roomIndex].changeId(newName);

    set<string> oldConn = connections[oldId];
    connections.erase(oldId);
    connections[newName] = oldConn;

    for(auto &conn : connections)
    {
        if(conn.second.count(oldId))
        {
           conn.second.erase(oldId);
           conn.second.insert(newName);
        }
    }
}

void Design::addConnection(const string &id1, const string &id2) {
    connections[id1].insert(id2);
    connections[id2].insert(id1);
}

void Design::scaleDesign(double unit)
{
    initializeMainDesign();
    for(auto &room : rooms)
    {
        room.scaleRoom(unit);
        updateMainDesign(room);
    }
}

void Design::moveDesign(double xChanged, double yChanged)
{
    //Initialize main Design
    initializeMainDesign();

    for(auto &room : rooms)
    {
        room.moveRoom(xChanged , yChanged );
        updateMainDesign(room);
    }
}


void Design::moveDesign(const Point &newPoint)
{
    double x1 = mainDesign.getX1() , y1 = mainDesign.getY1();
    double xChanged = newPoint.getX() - x1 , yChanged = newPoint.getY() - y1;

    //Initialize main Design
    initializeMainDesign();

    for(auto &room : rooms)
    {
        room.moveRoom(xChanged , yChanged);
        updateMainDesign(room);
    }
}

void Design::moveDesign(const Point &newPoint , const Point &basePoint)
{
    double x1 = basePoint.getX() , y1 = basePoint.getY();
    double xChanged = newPoint.getX() - x1 , yChanged = newPoint.getY() - y1;

    //Initialize main Design
    initializeMainDesign();

    for(auto &room : rooms)
    {
        room.moveRoom(xChanged , yChanged);
        updateMainDesign(room);
    }
}

void Design::updateMainDesign(const Room &newAddedRoom) {
    double x1 = newAddedRoom.getX1() , y1 = newAddedRoom.getY1();
    double x2 = newAddedRoom.getX2() , y2 = newAddedRoom.getY2();

    mainDesign.setX1(min(mainDesign.getX1() , x1));
    mainDesign.setY1(min(mainDesign.getY1() , y1));
    mainDesign.setX2(max(mainDesign.getX2() , x2));
    mainDesign.setY2(max(mainDesign.getY2() , y2));
}

void Design::updateMainDesign() {
    for(auto &room : rooms)
    {
        double x1 = room.getX1() , y1 = room.getY1();
        double x2 = room.getX2() , y2 = room.getY2();

        mainDesign.setX1(min(mainDesign.getX1() , x1));
        mainDesign.setY1(min(mainDesign.getY1() , y1));
        mainDesign.setX2(max(mainDesign.getX2() , x2));
        mainDesign.setY2(max(mainDesign.getY2() , y2));
    }
}

int Design::getSpacesCount() const {
    return spacesCount;
}

void Design::setSpacesCount(int _spacesCount) {
    Design::spacesCount = _spacesCount;
}

int Design::getBedroomsCount() const {
    return bedroomsCount;
}

void Design::setBedroomsCount(int _bedroomsCount) {
    Design::bedroomsCount = _bedroomsCount;
}

int Design::getRoomsCount() const {
    return roomsCount;
}

void Design::setRoomsCount(int _roomsCount) {
    Design::roomsCount = _roomsCount;
}

double Design::getArea() const {
    return area;
}

void Design::setArea(double _area) {
    Design::area = MathUtils::roundingToDecimal(_area);
}

const vector<Room> &Design::getRooms() const {
    return rooms;
}

Room &Design::getMainDesign() {
    updateMainDesign();
    return mainDesign;
}

void Design::initializeMainDesign() {
    mainDesign = roomBuilder.createRoom("Design" , 10000 , 10000, -10000 , -10000);
}
void Design::updateRoomDimensions(const string &roomId, double width, double height) {
    if(mapIdToIndex.count(roomId))
    {
        int roomIndex = mapIdToIndex[roomId];
        rooms[roomIndex].setX2(rooms[roomIndex].getX1() + width);
        rooms[roomIndex].setY2(rooms[roomIndex].getY1() + height);
    }
}

Room Design::fetchRoomById(const string &roomId) {
    if(mapIdToIndex.count(roomId))
    {
        int roomIndex = 0;
        for(auto &room : rooms)
        {
            if(room.getRoomId() == roomId)
            {
                break;
            }
            roomIndex++;
        }
        return rooms[roomIndex];
    }
    return {};
}

void Design::clearRoom() {
    rooms.clear();
    mapIdToIndex .clear();
    spacesCount = 0;
}

void Design::rotate(double angle)
{
    if (angle == 0)return;
    double MINX = 0, MINY = 0;
    for(auto &room : rooms)
    {
        room.rotate(angle);
    }
    for(auto &rec:rooms){
        MINX = min(MINX, rec.getX1());
        MINX = min(MINX, rec.getX2());
        MINY = min(MINY, rec.getY1());
        MINY = min(MINY, rec.getY2());
    }

    MINX = abs(MINX);
    MINY = abs(MINY);

    for(auto &rec:rooms)
    {
        double x1New = rec.getX1() +MINX;
        double y1New = rec.getY1() +MINY;
        double x2New = rec.getX2() +MINX;
        double y2New = rec.getY2() +MINY;

        x1New = round( x1New * 10 ) / 10 ;
        y1New = round( y1New * 10 ) / 10 ;
        x2New = round( x2New * 10 ) / 10 ;
        y2New = round( y2New * 10 ) / 10 ;

        rec.setX1(x1New);
        rec.setX2(x2New);
        rec.setY1(y1New);
        rec.setY2(y2New);

        if(rec.getX1() > rec.getX2()){
            double x = rec.getX1();
            rec.setX1(rec.getX2());
            rec.setX2(x);
        }
        if(rec.getY1() > rec.getY2()){
            double y = rec.getY1();
            rec.setY1(rec.getY2());
            rec.setY2(y);
        }

         for(auto &window:rec.getWindows()){
             Line line = window.getWindow();

             x1New = line.getX1() +MINX;
             y1New = line.getY1() +MINY;
             x2New = line.getX2() +MINX;
             y2New = line.getY2() +MINY;

             x1New=round(x1New*10)/10;
             y1New=round(y1New*10)/10;
             x2New=round(x2New*10)/10;
             y2New=round(y2New*10)/10;

             line.setX1(x1New);
             line.setX2(x2New);
             line.setY1(y1New);
             line.setY2(y2New);

             if(line.getX1() > line.getX2()){
                 double x = line.getX1();
                 line.setX1(line.getX2());
                 line.setX2(x);
             }
             if(line.getY1() > line.getY2()){
                 double y = line.getY1();
                 line.setY1(line.getY2());
                 line.setY2(y);
             }
             window .setWindow(line);
         }

         for(auto &door:rec.getDoors()){
             Line line = door.getDoor();

             x1New = line.getX1() +MINX;
             y1New = line.getY1() +MINY;
             x2New = line.getX2() +MINX;
             y2New = line.getY2() +MINY;

             x1New=round(x1New*10)/10;
             y1New=round(y1New*10)/10;
             x2New=round(x2New*10)/10;
             y2New=round(y2New*10)/10;

             line.setX1(x1New);
             line.setX2(x2New);
             line.setY1(y1New);
             line.setY2(y2New);

             if(line.getX1() > line.getX2()){
                 double x = line.getX1();
                 line.setX1(line.getX2());
                 line.setX2(x);
             }
             if(line.getY1() > line.getY2()){
                 double y = line.getY1();
                 line.setY1(line.getY2());
                 line.setY2(y);
             }
             door .setDoor(line);
         }

         for(auto &openning:rec.getOpenings()){
             Line line = openning.getOpening();

             x1New = line.getX1() +MINX;
             y1New = line.getY1() +MINY;
             x2New = line.getX2() +MINX;
             y2New = line.getY2() +MINY;

             x1New=round(x1New*10)/10;
             y1New=round(y1New*10)/10;
             x2New=round(x2New*10)/10;
             y2New=round(y2New*10)/10;

             line.setX1(x1New);
             line.setX2(x2New);
             line.setY1(y1New);
             line.setY2(y2New);

             if(line.getX1() > line.getX2()){
                 double x = line.getX1();
                 line.setX1(line.getX2());
                 line.setX2(x);
             }
             if(line.getY1() > line.getY2()){
                 double y = line.getY1();
                 line.setY1(line.getY2());
                 line.setY2(y);
             }
             openning .setOpening(line);
         }
    }

    updateMainDesign();
}


void Design::mirrorRecX() {
    for (auto &room: rooms) {
        room.mirrorX();
    }
    moveDesign({0,0});
}

void Design::mirrorRecY() {
    for (auto &room: rooms) {
        room.mirrorY();
    }
    moveDesign({0,0});
}

vector<Room> &Design::getRoomsRef() {
    return rooms;
}

void Design::addWindows()
{
    double windowLength = 1;
    for(auto &prev : rooms)
    {
        char prevFace = roomsIDSFaces[prev.getRoomId()];
        vector<char>directions ;
        if(prevFace == 'N' || prevFace=='S' || prevFace=='E' || prevFace=='W')
        {
            directions.push_back(prevFace);
        }
        else{
            directions = vector<char>{'N' , 'S' , 'E' , 'W'};
        }
        for(auto &face : directions)
        {
            prevFace = face;
            vector<Line>prevLines;
            bool addedWindow = false;
            if(prevFace == 'N' || prevFace == 'S')
            {
                double sharedY = prevFace == 'N' ? prev.getY1() : prev.getY2();
                for(auto &room : rooms)
                {
                    if(room.getRoomId() == prev.getRoomId())continue;
                    double minX = max(room.getX1() , prev.getX1()) ;
                    double maxX = min(room.getX2() , prev.getX2()) ;
                    //line in left or right room
                    if(minX > prev.getX2() || maxX < prev.getX1()) continue;

                    if(room.getY1() == sharedY || room.getY2() == sharedY)
                    {
                        prevLines.emplace_back(minX , sharedY , maxX , sharedY);
                    }
                }
                sort(prevLines.begin() , prevLines.end());
                double start = prev.getX1();
                for(auto &line : prevLines)
                {
                    if(line.getX1() - start >= 2.25 )windowLength = 2;
                    else windowLength = 1;

                    if(line.getX1() - start >= 1.5)
                    {
                        double startX = ((line.getX1() - start ) / 2) - (windowLength/2);
                        startX = round((startX + start) * 10 )/10;
                        double wX2 = round((startX+windowLength) * 10) / 10;
                        prev.addWindow(startX , sharedY , wX2 , sharedY);

                        start = prev.getX2();
                        addedWindow = true;
                        break;
                    }
                    start = line.getX2();
                }
                if(addedWindow)break;

                if(prev.getX2() - start >= 2.25 )windowLength = 2;
                else windowLength = 1;

                if( prev.getX2() - start >= 1.5)
                {
                    double startX = ((prev.getX2() - start ) / 2) - (windowLength/2);
                    startX = round((startX + start) * 10 )/10;
                    double wX2 = round((startX+windowLength) * 10) / 10;
                    prev.addWindow(startX , sharedY , wX2 , sharedY);
                    addedWindow = true;
                    break;
                }
            }
            else if(prevFace == 'E' || prevFace == 'W')
            {
                double sharedX = prevFace == 'E' ? prev.getX2() : prev.getX1();
                for(auto &room : rooms)
                {
                    if(room.getRoomId() == prev.getRoomId()) continue;
                    double minY = max(room.getY1() , prev.getY1());
                    double maxY = min(room.getY2() , prev.getY2());


                    //line in left or right room
                    if(minY > prev.getY2() || maxY < prev.getY1()) continue;

                    if(room.getX1() == sharedX || room.getX2() == sharedX)
                    {
                        prevLines.emplace_back(sharedX , minY , sharedX , maxY);
                    }
                }
                sort(prevLines.begin() , prevLines.end());
                double start = prev.getY1();
                for(auto &line : prevLines)
                {
                    if(line.getY1() - start >= 2.25 )windowLength = 2;
                    else windowLength = 1;

                    if(line.getY1() - start >= 1.5)
                    {
                        double startY = ((line.getY1() - start) / 2) - windowLength/2;
                        startY = round((startY + start) * 10 )/10;
                        double wY2 = round((startY+windowLength) * 10) / 10;
                        prev.addWindow(sharedX , startY , sharedX , wY2);
                        start = prev.getY2();
                        addedWindow = true;
                        break;
                    }
                    start = line.getY2();
                }
                if(addedWindow)break;

                if(prev.getY2() - start >= 2.25 )windowLength = 2;
                else windowLength = 1;

                if(prev.getY2() - start >= 1.5)
                {
                    double startY = ((prev.getY2() - start) / 2) - windowLength/2;
                    startY = round((startY + start) * 10 )/10;
                    double wY2 = round((startY+windowLength) * 10) / 10;
                    prev.addWindow(sharedX , startY , sharedX , wY2);
                    addedWindow = true;
                    break;
                }
            }
        }

    }
}

void Design::addDoors()
{
    for (int i = 0; i < rooms.size(); ++i)
    {
        Room &firstRoom =  rooms[i];

        if(firstRoom.getRoomType() == RoomType::Living || firstRoom.getRoomType() == RoomType::FamilyRoom) continue;

        for (int j = i+1; j < rooms.size(); ++j)
        {
            Room &secondRoom = rooms[j];

            if(secondRoom.getRoomType() == RoomType::Living || secondRoom.getRoomType() == RoomType::FamilyRoom) continue;

            if(!connections[firstRoom.getRoomId()] . count(secondRoom.getRoomId())) continue;

            double sharedX = -1 , sharedY = -1;
            if(firstRoom.getX1() == secondRoom.getX2())
            {
                sharedX = firstRoom.getX1();
            }

            else if(firstRoom.getX2() == secondRoom.getX1())
            {
                sharedX = firstRoom.getX2();
            }

            else if(firstRoom.getY1() == secondRoom.getY2())
            {
                sharedY = firstRoom.getY1();
            }

            else if(firstRoom.getY2() == secondRoom.getY1())
            {
                sharedY = firstRoom.getY2();

            }

            if(sharedX != -1)
            {
                double y1 = max(firstRoom.getY1() , secondRoom.getY1());
                double y2 = min(firstRoom.getY2() , secondRoom.getY2());
                if(y1 < y2 && y2 - y1 >= 1.5)
                {

                    if(firstRoom.getRoomType() == RoomType::Bathroom || firstRoom.getRoomType() == RoomType::GuestBathroom)
                    {
                        firstRoom.addDoor(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                    }

                    else if(secondRoom.getRoomType() == RoomType::Bathroom || secondRoom.getRoomType() == RoomType::GuestBathroom)
                    {
                        secondRoom.addDoor(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                    }

                    else if(firstRoom.getRoomType() == RoomType::Dining)
                    {
                        secondRoom.addDoor(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                    }
                    else if(secondRoom.getRoomType() == RoomType::Dining)
                    {
                        firstRoom.addDoor(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                    }
                    else {
                        secondRoom.addDoor(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                    }
                }
            }

            if(sharedY != -1)
            {
                double x1 = max(firstRoom.getX1() , secondRoom.getX1());
                double x2 = min(firstRoom.getX2() , secondRoom.getX2());
                if(x1 < x2 && x2 - x1 >= 1.5)
                {
                    if(firstRoom.getRoomType() == RoomType::Bathroom || firstRoom.getRoomType() == RoomType::GuestBathroom)
                    {
                        firstRoom.addDoor(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                    }

                    else if(secondRoom.getRoomType() == RoomType::Bathroom || secondRoom.getRoomType() == RoomType::GuestBathroom)
                    {
                        secondRoom.addDoor(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                    }
                    else if(firstRoom.getRoomType() == RoomType::Dining)
                    {
                        secondRoom.addDoor(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                    }
                    else if(secondRoom.getRoomType() == RoomType::Dining)
                    {
                        firstRoom.addDoor(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                    }
                    else {
                        secondRoom.addDoor(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                    }
                }
            }
        }
    }
}

void Design::addOpenings()
{
    for (int i = 0; i < rooms.size(); ++i)
    {
        Room &firstRoom =  rooms[i];
        if(firstRoom.getRoomType() == RoomType::Bathroom || firstRoom.getRoomType()== RoomType::GuestBathroom) continue;

        for (int j = i+1; j < rooms.size(); ++j)
        {
            Room &secondRoom = rooms[j];

            if(secondRoom.getRoomType() == RoomType::Bathroom || secondRoom.getRoomType()== RoomType::GuestBathroom) continue;

            if(!(firstRoom.getRoomType() == RoomType::Living || secondRoom.getRoomType() == RoomType::Living || secondRoom.getRoomType() == RoomType::FamilyRoom || firstRoom.getRoomType() == RoomType::FamilyRoom))continue;

            if(!connections[firstRoom.getRoomId()] . count(secondRoom.getRoomId())) continue;

            double sharedX = -1 , sharedY = -1;
            if(firstRoom.getX1() == secondRoom.getX2())
            {
                sharedX = firstRoom.getX1();
            }

            else if(firstRoom.getX2() == secondRoom.getX1())
            {
                sharedX = firstRoom.getX2();
            }

            else if(firstRoom.getY1() == secondRoom.getY2())
            {
                sharedY = firstRoom.getY1();
            }

            else if(firstRoom.getY2() == secondRoom.getY1())
            {
                sharedY = firstRoom.getY2();

            }

            if(sharedX != -1)
            {
                double y1 = max(firstRoom.getY1() , secondRoom.getY1());
                double y2 = min(firstRoom.getY2() , secondRoom.getY2());
                if(y1 < y2 && y2 - y1 >= 1.5)
                {
                    if(firstRoom.getRoomType() != RoomType::Corridor && secondRoom.getRoomType() != RoomType::Corridor )
                    {
                        firstRoom.addOpening(sharedX , y1 + 0.2 , sharedX , y2 - 0.2);
                    }
                    else secondRoom.addOpening(sharedX , y1 + ((y2-y1) / 2) - 0.5 , sharedX , y1 + ((y2-y1) / 2) + 0.5);
                }
            }

            if(sharedY != -1)
            {
                double x1 = max(firstRoom.getX1() , secondRoom.getX1());
                double x2 = min(firstRoom.getX2() , secondRoom.getX2());
                if(x1 < x2 && x2 - x1 >= 1.5)
                {
                    if(firstRoom.getRoomType() != RoomType::Corridor && secondRoom.getRoomType() != RoomType::Corridor )
                    {
                        firstRoom.addOpening(x1 + 0.2 , sharedY ,  x2 - 0.2 , sharedY);
                    }
                    else secondRoom.addOpening(x1 + ((x2-x1) / 2) - 0.5 , sharedY ,  x1 + ((x2-x1) / 2) + 0.5 , sharedY);
                }
            }
        }
    }
}

const map<string, char> &Design::getRoomsIdsFaces() const {
    return roomsIDSFaces;
}

void Design::setRoomsIdsFaces(const map<string, char> &roomsIdsFaces) {
    roomsIDSFaces = roomsIdsFaces;
}

const map<string, int> &Design::getRoomsIdsLayer() const {
    return roomsIDSLayer;
}

void Design::setRoomsIdsLayer(const map<string, int> &roomsIdsLayer) {
    roomsIDSLayer = roomsIdsLayer;
}

void Design::setConnections(const map<string, set<string>> &connections) {
    Design::connections = connections;
}

void Design::updateRoomDimensions(const string &roomId, double x1, double y1, double x2, double y2)
{
    if(mapIdToIndex.count(roomId))
    {
        int roomIndex = 0;
        for(auto &room : rooms)
        {
            if(room.getRoomId() == roomId)
            {
                break;
            }
            roomIndex++;
        }
        rooms[roomIndex].setX1(x1);
        rooms[roomIndex].setY1(y1);
        rooms[roomIndex].setX2(x2);
        rooms[roomIndex].setY2(y2);
    }
}

void Design::shiftDesignX(double xUnits)
{
    for(auto &room : rooms)room.shiftX(xUnits);
    updateMainDesign();
}

void Design::shiftDesignY(double yUnits)
{
    for(auto &room : rooms)room.shiftY(yUnits);
    updateMainDesign();
}

vector<Wall> Design::getWallFraming()
{
    Design newD = *this;
    return DesignWallGenerator::generateWalls(newD);
}


