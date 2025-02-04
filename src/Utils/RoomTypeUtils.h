//
// Created by ahmed Ibrahim on 26-Nov-24.
//

#ifndef DESIGNI_ROOMTYPEUTILS_H
#define DESIGNI_ROOMTYPEUTILS_H
#include "src/Model/RoomTypes.h"

class RoomTypeUtils {
public:
    /**
     * Convert Room Type to String
     * @param roomType
     * @return
     */
    static string parseRoomTypeToString(RoomType roomType);

    /**
     * Convert string type to match Room Type
     * @param type
     * @return
     */
    static RoomType parseStringToRoomType(const string &type);

    /**
     * Normalize Room Type String to base RoomType without un needed suffix or prefix
     * @param type
     * @return
     */
    static string normalizeRoomTypeString(const string &type);
};


#endif //DESIGNI_ROOMTYPEUTILS_H
