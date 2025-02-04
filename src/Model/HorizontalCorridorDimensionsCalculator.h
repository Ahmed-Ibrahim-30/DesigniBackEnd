//
// Created by ahmed Ibrahim on 02-Dec-24.
//

#ifndef DESIGNI_HORIZONTALCORRIDORDIMENSIONSCALCULATOR_H
#define DESIGNI_HORIZONTALCORRIDORDIMENSIONSCALCULATOR_H
#include "CorridorDimensionsCalculator.h"

/**
 * assign Rooms besides Horizontal Corridor ( width > height)
 */
class HorizontalCorridorDimensionsCalculator :public CorridorDimensionsCalculator{
public:

    /**
     * Connect Horizontal Corridor Rooms
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
    HorizontalCorridorDimensionsCalculator(const Design &inputDesign, const string &corridorName, Side blockSide);

protected:
    vector<vector<Group>>getGroupDivisions(int roomNum, vector<Group> &groups) override;

    /**
     * @brief getGroupsShapes
     * @param index
     * @param groups
     * @param design
     * @param newDesign
     * @return
     */
    vector<Design> getGroupsShapes(int index, vector<Group> &groups, Design &design , Design &newDesign) override;

    vector<Design> getGroupsShapes(int index,int room, vector<Group> &groups, Design &design , Design &newDesign) override;

    vector<Design> getGroupsShapesTwoSide(int index,int room, vector<Group> &groups, Design &design , Design &newDesign) override;

    /**
     * Sort Group Divisions based on diff area between top and bottom
     * @param groupDivisions
     * @return
     */
    vector<vector<Group>> sortGroupDivisions(vector<vector<Group>> groupDivisions);

    /**
     * Get Two Group (TOP,BOTTOM) ONLY
     * @param roomNum
     * @param groups
     * @return
     */
    vector<vector<Group>>getTwoGroupDivisions(int roomNum, vector<Group> &groups);
};


#endif //DESIGNI_HORIZONTALCORRIDORDIMENSIONSCALCULATOR_H
