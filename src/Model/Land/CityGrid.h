//
// Created by ahmed Ibrahim on 10-Mar-25.
//

#ifndef DESIGNI_BACKEND_CITYGRID_H
#define DESIGNI_BACKEND_CITYGRID_H
#include "src/Model/Line.h"

class CityGrid {
private:
    vector<Line> innerStreets , outerStreets;
    vector<Line> roadExtension;
    vector<Line> homeBorder;
public:
    CityGrid() = default;

    vector<Line> getStreets() ;

    void setInnerStreets(const vector<Line> &innerStreets);

    void setOuterStreets(const vector<Line> &outerStreets);

    const vector<Line> &getInnerStreets() const;

    const vector<Line> &getOuterStreets() const;

    void setStreets(const vector<Line> &inner , const vector<Line> &outer);

    [[nodiscard]] const vector<Line> &getRoadExtension() const;

    void setRoadExtension(const vector<Line> &roadExtension);

    [[nodiscard]] const vector<Line> &getHomeBorder() const;

    void setHomeBorder(const vector<Line> &homeBorder);

};


#endif //DESIGNI_BACKEND_CITYGRID_H
