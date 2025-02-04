//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#ifndef DESIGNI_DESIGNCOMPACTNESSREFACTOR_H
#define DESIGNI_DESIGNCOMPACTNESSREFACTOR_H
#include "DesignRefactor.h"

class DesignCompactnessRefactor :public DesignRefactor{
private:
    /**
     * Get all Empty Lines
     * @param rooms
     * @return
     */
    vector<Line> emptyWallsCalculation( const vector<Room> &rooms);

    /**
     * @brief getTwoRoomsRelatedToEmptyLine
     * @param shape
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return
     */
    pair<int , int> getTwoRoomsIndexRelatedToEmptyLine(Design &shape , double x1 , double y1 , double x2 , double y2);

    /**
     * Is Room is sutiable to change X OR Y dimesnions
     * @param shape
     * @param room
     * @param xRefactor
     * @param yRefactor
     * @return
     */
    bool validRefactor(const Design &shape ,const Room &room , bool x1Refactor ,bool x2Refactor, bool y1Refactor , bool y2Refactor);

    /**
     * Is Line is empty from two sides
     * @param lines
     * @param line
     * @return
     */
    bool isEmptyLineFromTwoSide(const vector<Line> &lines , const Line &line);
public:
    /**
     * @brief refactorShape (alignment)
     * @param shape
     */
    void refactorDesign(Design &shape) override;
};


#endif //DESIGNI_DESIGNCOMPACTNESSREFACTOR_H
