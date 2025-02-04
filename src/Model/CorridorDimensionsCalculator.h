//
// Created by ahmed Ibrahim on 02-Dec-24.
//

#ifndef DESIGNI_CORRIDORDIMENSIONSCALCULATOR_H
#define DESIGNI_CORRIDORDIMENSIONSCALCULATOR_H

#include <ostream>
#include "Design.h"
#include "AreaToDimensionsCalculator.h"
#include "DesignValidator.h"

enum class Side{
    TOP,
    BOTTOM,
    RIGHT,
    LEFT
};

class CorridorDimensionsCalculator {
protected:
    double maxCorridorWidth ; //Max Width for corridor
    double maxCorridorHeight ; // Max Height for corridor

    /**
     * determine should be rotate design 180 if it RIGHT , BOTTOM
     */
    double rotatedDegree;
    struct Group
    {
        vector<string> rooms;
        Side groupSide;

        explicit Group(const vector<string> &st , Side side)
        {
            rooms = st;
            groupSide = side;
        }
        Group() = default;

        bool operator<(const Group &rhs) const {
            if (rooms < rhs.rooms)
                return true;
            if (rhs.rooms < rooms)
                return false;
            return groupSide < rhs.groupSide;
        }

        bool operator>(const Group &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const Group &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const Group &rhs) const {
            return !(*this < rhs);
        }

        void print()
        {
            if(groupSide == Side::LEFT)cout<<"Side = Left\n";
            else if(groupSide == Side::RIGHT)cout<<"Side = RIGHT\n";
            else if(groupSide == Side::TOP)cout<<"Side = TOP\n";
            else if(groupSide == Side::BOTTOM)cout<<"Side = BOTTOM\n";

            for(auto &st: rooms)
            {
                cout<<st << " ";
            }
            cout<<endl;
        }

    };

    string corridorName;

    map<string, set<string> > roomsConnections;

    /**
     * all Rooms connected to Corridor
     */
    vector<string> corridorRoomsIds;

    /**
     * Map each Room Id to corresponding Area
     */
    map<string, double> roomsIDSArea;

    /**
     * Rules always assume that One Room will connect to Corridor and this one in this class either right of left corridor
     */
    Side blockSide;

    /**
     * Get Group Divisons
     * @param roomNum
     * @param roomsIds
     * @param groups
     * @return
     */
    virtual vector<vector<Group>> getGroupDivisions(int roomNum , vector<Group>&groups) = 0;

    /**
     * Get Groups Shapes
     * @param index
     * @param groups
     * @param design
     * @param newDesign
     * @return
     */
    virtual vector<Design> getGroupsShapes(int index, vector<Group> &groups, Design &design , Design &newDesign) = 0;

    virtual vector<Design> getGroupsShapes(int index,int room, vector<Group> &groups, Design &design , Design &newDesign) =0;

    virtual vector<Design> getGroupsShapesTwoSide(int index,int room, vector<Group> &groups, Design &design , Design &newDesign) = 0;

public:
    CorridorDimensionsCalculator(const Design &inputDesign , const string &_corridorName , Side _blockSide){
        corridorName = _corridorName;
        roomsConnections = inputDesign.getConnections();
        blockSide = _blockSide;
        rotatedDegree = 0.0;

        for(auto &room : inputDesign.getRooms())
        {
            roomsIDSArea[room.getRoomId()] = room.getArea();
        }

        for(auto &conn : roomsConnections[corridorName])
        {
            corridorRoomsIds.push_back(conn);
        }
    }

    /**
     * Connect Corridor Rooms with Corridor
     * @param outputDesigns
     * @return
     */
    virtual vector<Design> ConnectCorridorRooms( Design &outputDesigns) = 0;
};


#endif //DESIGNI_CORRIDORDIMENSIONSCALCULATOR_H
