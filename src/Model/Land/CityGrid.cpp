//
// Created by ahmed Ibrahim on 10-Mar-25.
//

#include "CityGrid.h"

vector<Line> CityGrid::getStreets()
{
    vector<Line> streets1 = innerStreets;
    streets1.insert(streets1.end() , outerStreets.begin() , outerStreets.end());
    return streets1;
}

void CityGrid::setStreets(const vector<Line> &inner , const vector<Line> &outer) {
    setInnerStreets(inner);
    setOuterStreets(outer);
}

const vector<Line> &CityGrid::getRoadExtension() const {
    return roadExtension;
}

void CityGrid::setRoadExtension(const vector<Line> &roadExtension) {
    CityGrid::roadExtension = roadExtension;
}

const vector<Line> &CityGrid::getHomeBorder() const {
    return homeBorder;
}

void CityGrid::setHomeBorder(const vector<Line> &homeBorder) {
    CityGrid::homeBorder = homeBorder;
}

const vector<Line> &CityGrid::getInnerStreets() const {
    return innerStreets;
}

const vector<Line> &CityGrid::getOuterStreets() const {
    return outerStreets;
}

void CityGrid::setInnerStreets(const vector<Line> &innerStreets) {
    CityGrid::innerStreets = innerStreets;
}

void CityGrid::setOuterStreets(const vector<Line> &outerStreets) {
    CityGrid::outerStreets = outerStreets;
}
