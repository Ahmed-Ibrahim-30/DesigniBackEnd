//
// Created by ahmed Ibrahim on 27-Nov-24.
//

#include "ConstInputView.h"

Design ConstInputView::getUserInput() {
    Design design;

    RoomBuilder roomBuilder;

    Room bedroom =  roomBuilder.createRoom("Bedroom1" , 0 , 0 , 5 , 10);
    Room living =  roomBuilder.createRoom("Living1" , 5 , 0 , 10 , 10);
    Room dining =  roomBuilder.createRoom("Dining1" , 0 , 10 , 10 , 15);

    design.addRoom(bedroom);
    design.addRoom(living);
    design.addRoom(dining);

    return design;
}
