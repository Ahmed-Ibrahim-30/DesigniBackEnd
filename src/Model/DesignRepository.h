//
// Created by ahmed Ibrahim on 08-Dec-24.
//

#ifndef DESIGNI_DESIGNREPOSITORY_H
#define DESIGNI_DESIGNREPOSITORY_H
#include "Design.h"

/**
 * Design Repository to manage Design demands
 */
class DesignRepository
{

private:

    vector<Design> designs;

    /**
     * map each Design index to corresponding number of spaces
     */
    map<int , vector<int>> mapDesignIndexToSpaces;

    /**
     * map each Design index to corresponding number of Bedrooms
     */
    map<int , vector<int>> mapDesignIndexToBedrooms;

    /**
     * map each Design index to corresponding number of Rooms
     */
    map<int , vector<int>> mapDesignIndexToRooms;

    /**
     * vector of All Designs with corresponding area
     */
    map<double , vector<int>> mapDesignIndexToArea;

    /**
     * Map Bedrooms To Solutions Index
     */
    map<int , int> mapBedroomsToSolIndex;

    /**
     * Map Rooms To Solutions Index
     */
    map<int , int> mapRoomsToSolIndex;

    /**
     * Map Spaces To Solutions Index
     */
    map<int , int> mapSpacesToSolIndex;

    /**
     * Map Area To Solutions Index
     */
    map<double , int> mapAreaToSolIndex;
public:

    /**
     * add New Design
     * @param design
     */
    virtual void addDesign(const Design &design) ;

    /**
     * Get Design By Number of spaces
     * @param numberOfSpaces
     * @return
     */
    virtual Design getDesignBySpaces(int numberOfSpaces);

    /**
     * @brief scaleDesigns
     * @param scaleUnit
     */
    virtual void scaleDesigns(double scaleUnit);

    /**
     * Get Design By Number of bedrooms
     * @param numberOfBedrooms
     * @return
     */
    virtual Design getDesignByBedrooms(int numberOfBedrooms);

    /**
     * Get Design By Number of rooms
     * @param numberOfRooms
     * @return
     */
    virtual Design getDesignByRooms(int numberOfRooms);

    /**
     * Get Design By Specific Area
     * @param area
     * @return
     */
    virtual Design getDesignByArea(double area);

    /**
     * Get Designs
     * @return
     */
    [[nodiscard]] const vector<Design> &getDesigns() const;
};
#endif //DESIGNI_DESIGNREPOSITORY_H
