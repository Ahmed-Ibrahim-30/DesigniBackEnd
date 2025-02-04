//
// Created by ahmed Ibrahim on 01-Dec-24.
//

#ifndef DESIGNI_ROOMCOREANALYZER_H
#define DESIGNI_ROOMCOREANALYZER_H
#include "Design.h"

class RoomCoreAnalyzer {
public:
    /**
     * Get All Possible cores in current Design
     * @param design
     * @return
     */
    static vector<string> calculateRoomCores(const Design &design);
};


#endif //DESIGNI_ROOMCOREANALYZER_H
