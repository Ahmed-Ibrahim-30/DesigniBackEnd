//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_UNIQUEIDGENERATOR_H
#define DESIGNI_UNIQUEIDGENERATOR_H
#include "src/Model/RoomTypes.h"
#include "RoomTypeUtils.h"
/**
 * Class UniqueIdGenerator Responsible for Generate Unique Id to Room
 * Class UniqueIdGenerator apply Singleton pattern
 */
class UniqueIdGenerator {
private:
    /**
     * Counter to count each Room Type
     */
    unordered_map<RoomType , int> roomTypeCounter;

    /**
     * All Stored Room Ids
     */
    set<string> roomIds;

    /**
     * instance from UniqueIdGenerator
     */
    static UniqueIdGenerator * uniqueIdGenerator;

    /*
     * Mutex to ensure thread safety
     */
    static mutex uniqueMutex;
    /**
     * private Constructor
     */
    UniqueIdGenerator();
public:
    /**
     * Deleting the copy constructor to prevent copies
     */
    UniqueIdGenerator(const UniqueIdGenerator &) = delete;
    /**
     * get Unique Instance from UniqueIdGenerator
     * @return
     */
    static UniqueIdGenerator* getInstance();

    /**
     * Generate Unique Id
     * @return
     */
     string generateId( const string &roomName);

     /**
      * Generate Id for rooms with type Other
      * @param roomName
      * @return
      */
     string generateOtherId(const string &roomName);
};


#endif //DESIGNI_UNIQUEIDGENERATOR_H
