//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#include "UniqueIdGenerator.h"
UniqueIdGenerator* UniqueIdGenerator ::uniqueIdGenerator = nullptr;
mutex UniqueIdGenerator::uniqueMutex;

string UniqueIdGenerator::generateId( const string &roomName) {
    string newId ;

    newId = roomName;

    // if(roomIds.count(roomName)) newId += "1";

    roomIds.insert(newId);
    return newId;
}

UniqueIdGenerator* UniqueIdGenerator::getInstance() {
    if(uniqueIdGenerator == nullptr)
    {
        lock_guard<mutex> lock(uniqueMutex);
        if(uniqueIdGenerator == nullptr)
        {
            uniqueIdGenerator = new UniqueIdGenerator();
        }
    }
    return uniqueIdGenerator;
}

UniqueIdGenerator::UniqueIdGenerator() = default;
