//
// Created by ahmed Ibrahim on 11-Dec-24.
//

#ifndef DESIGNI_DESIGNCOMPACTNESSSORTER_H
#define DESIGNI_DESIGNCOMPACTNESSSORTER_H
#include "DesignsSorting.h"

/**
 * Sort Designs based on shared Walls
 */
class DesignCompactnessSorter : public DesignsSorting{
private:
    double emptyWallsCalculation(const vector<Room> &rooms);
public:
    /**
     * Design
     * @param design
     */
    void sortDesigns(vector<Design> &design) override;
};


#endif //DESIGNI_DESIGNCOMPACTNESSSORTER_H
