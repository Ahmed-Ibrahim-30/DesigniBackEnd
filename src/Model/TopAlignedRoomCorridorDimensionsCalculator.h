//
// Created by ahmed Ibrahim on 15-Dec-24.
//

#ifndef DESIGNI_TOPALIGNEDROOMCORRIDORDIMENSIONSCALCULATOR_H
#define DESIGNI_TOPALIGNEDROOMCORRIDORDIMENSIONSCALCULATOR_H
#include "CorridorDimensionsCalculator.h"

class TopAlignedRoomCorridorDimensionsCalculator :public CorridorDimensionsCalculator{
public:
    /**
     * Connect Top Aligned Room by Corridor.
     * @param inputDesign
     * @param _corridorName
     * @param blockSide  : Either Right Of Left
     * @param outputDesigns
     * @return
     */
    vector<Design> ConnectCorridorRooms(Design &outputDesigns) override;

    /**
     * Parameterized  constructor
     * @param inputDesign
     * @param corridorName
     * @param blockSide
     */
    TopAlignedRoomCorridorDimensionsCalculator(const Design &inputDesign, const string &corridorName, Side blockSide);

protected:
    /**
     * get Room Divisions
     * @param roomNum
     * @param groups
     * @return
     */
    vector<vector<Group>> getGroupDivisions(int roomNum, vector<Group> &groups) override;

    vector<Design> getGroupsShapes(int index, vector<Group> &groups, Design &design, Design &newDesign) override;

    vector<Design>
    getGroupsShapes(int index, int room, vector<Group> &groups, Design &design, Design &newDesign) override;

    vector<Design>
    getGroupsShapesTwoSide(int index, int room, vector<Group> &groups, Design &design, Design &newDesign) override;
};


#endif //DESIGNI_TOPALIGNEDROOMCORRIDORDIMENSIONSCALCULATOR_H
