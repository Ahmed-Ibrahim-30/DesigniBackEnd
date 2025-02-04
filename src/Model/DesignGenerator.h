//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_DESIGNGENERATOR_H
#define DESIGNI_DESIGNGENERATOR_H

#include "Design.h"
#include "RoomConnectionManager.h"
#include "RoomCoreAnalyzer.h"
#include "CorridorConnectionManager.h"
#include "DesignCompactnessSorter.h"
#include "DesignCompactnessRefactor.h"
/**
 * Responsible for generate new Design based on inputs
 */
class DesignGenerator {
private:
    RoomConnectionManager roomConnectionManager;
    CorridorConnectionManager corridorConnectionManager;
    DesignsSorting* designsSorting ;
    DesignRefactor* designRefactor;
public:
    /**
     * Generate list of designs
     * @param design
     * @return
     */
    vector<Design> generateDesign(const Design &design);

    vector<Design> generateDesign1(const Design &design);

    /**
     * @brief generateDesign
     * @return
     */
    vector<Design> generateDesign(const map<string , double> &mapRoomsWidthAreaLivingCore , const map<string , double> &mapRoomsWidthAreaBedroomCore , const map<string , double> &mapRoomsWidthAreaFamilyCore);

    DesignGenerator () = default;
};


#endif //DESIGNI_DESIGNGENERATOR_H
