//
// Created by ahmed Ibrahim on 07-Feb-25.
//

#ifndef DESIGNI_BACKEND_TEMPLATEDESIGNS_H
#define DESIGNI_BACKEND_TEMPLATEDESIGNS_H
#include "src/Model/Design.h"

class TemplateDesigns {
private:
    void B(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B2L1(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B3L2(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B3L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B4L3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B5L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A1(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A2(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L4A3(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A4(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A5(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);
    void B6L5A6(const vector<Design>&livingShapes,const vector<Design>&bedroomShapes,const vector<Design>&familyShapes);

    const string familyImagePath = "Images/AZone/";
    const string livingImagePath = "Images/LZone/";
    const string bedroomimagePath= "Images/BZone/";
};


#endif //DESIGNI_BACKEND_TEMPLATEDESIGNS_H
