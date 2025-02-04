//
// Created by ahmed Ibrahim on 25-Nov-24.
//

#ifndef DESIGNI_USERINPUTVIEW_H
#define DESIGNI_USERINPUTVIEW_H

#include "src/Model/Design.h"
#include "src/Model/RoomBuilder.h"

class UserInputView {
public:
    virtual Design getUserInput() = 0;
};


#endif //DESIGNI_USERINPUTVIEW_H
