//
// Created by ahmed Ibrahim on 30-Nov-24.
//

#include "TemplateRepository.h"
RoomBuilder TemplateRepository::roomBuilder;
vector<Design> TemplateRepository::templatesCreations() {
    vector<Design>templates;

    templates.push_back(createTemplate1());
    templates.push_back(createTemplate2());
    templates.push_back(createTemplate3());
    templates.push_back(createTemplate4());
    templates.push_back(createTemplate5());
    templates.push_back(createTemplate6());
    templates.push_back(createTemplate7());
    templates.push_back(createTemplate8());
    templates.push_back(createTemplate9());
    templates.push_back(createTemplate10());
    templates.push_back(createTemplate11());
    templates.push_back(createTemplate12());
    templates.push_back(createTemplate13());
    templates.push_back(createTemplate14());
    templates.push_back(createTemplate15());
    templates.push_back(createTemplate16());
    templates.push_back(createTemplate17());
    templates.push_back(createTemplate18());

    return templates;
}

Design TemplateRepository::createTemplate1() {
    Design templateDesign;

    templateDesign.setArea(26);
    templateDesign.setBedroomsCount(1);
    templateDesign.setRoomsCount(1);

    Room bedroom1  = roomBuilder.createRoom("Bedroom" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );

    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bath1);

    templateDesign.addConnection("Bedroom" , "Bath");
    return templateDesign;
}

Design TemplateRepository::createTemplate2()
{
    Design templateDesign;

    templateDesign.setArea(42);
    templateDesign.setBedroomsCount(1);
    templateDesign.setRoomsCount(2);

    Room bedroom1  = roomBuilder.createRoom("Bedroom" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 16 );

    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(living);

    templateDesign.addConnection("Bedroom" , "Bath");
    templateDesign.addConnection("Bedroom" , "Living");
    templateDesign.addConnection("Living" , "Bath");
    return templateDesign;
}

Design TemplateRepository::createTemplate3()
{
    Design templateDesign;

    templateDesign.setArea(52);
    templateDesign.setBedroomsCount(1);
    templateDesign.setRoomsCount(2);

    Room bedroom1  = roomBuilder.createRoom("Bedroom" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 17 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 9 );

    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);

    templateDesign.addConnection("Bedroom" , "Bath");
    templateDesign.addConnection("Bedroom" , "Living");
    templateDesign.addConnection("Living" , "Bath");
    templateDesign.addConnection("Living" , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate4()
{
    Design templateDesign;

    templateDesign.setArea(58);
    templateDesign.setBedroomsCount(1);
    templateDesign.setRoomsCount(2);

    Room bedroom1  = roomBuilder.createRoom("Bedroom" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 18 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 10 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);

    templateDesign.addConnection("Bedroom" , "Bath");
    templateDesign.addConnection("Bedroom" , "Living");
    templateDesign.addConnection("Living" , "GB");
    templateDesign.addConnection("Kitchen" , "GB");
    templateDesign.addConnection("Living" , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate5()
{
    Design templateDesign;

    templateDesign.setArea(69);
    templateDesign.setBedroomsCount(1);
    templateDesign.setRoomsCount(3);

    Room bedroom1  = roomBuilder.createRoom("Bedroom" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 19 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 11 );
    Room dining   = roomBuilder.createRoom("Dining" , 12 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Bedroom" , "Bath");
    templateDesign.addConnection("Bedroom" , "Living");
    templateDesign.addConnection("Living" , "Dining");
    templateDesign.addConnection("Living" , "Kitchen");
    templateDesign.addConnection("GB" , "Dining");
    templateDesign.addConnection("Dining" , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate6()
{
    Design templateDesign;

    templateDesign.setArea(92);
    templateDesign.setBedroomsCount(2);
    templateDesign.setRoomsCount(4);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 20 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 12 );
    Room dining    = roomBuilder.createRoom("Dining" , 13 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Corridor" , "Bath");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate7()
{
    Design templateDesign;

    templateDesign.setArea(101);
    templateDesign.setBedroomsCount(2);
    templateDesign.setRoomsCount(4);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 21 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 12 );
    Room dining    = roomBuilder.createRoom("Dining" , 13 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate8()
{
    Design templateDesign;

    templateDesign.setArea(124);
    templateDesign.setBedroomsCount(3);
    templateDesign.setRoomsCount(5);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 22 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 13 );
    Room dining    = roomBuilder.createRoom("Dining" , 14 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Bedroom3");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate9()
{
    Design templateDesign;

    templateDesign.setArea(133);
    templateDesign.setBedroomsCount(3);
    templateDesign.setRoomsCount(5);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 21 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 13 );
    Room dining    = roomBuilder.createRoom("Dining" , 14 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bath3");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Bedroom3");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate10()
{
    Design templateDesign;

    templateDesign.setArea(156);
    templateDesign.setBedroomsCount(4);
    templateDesign.setRoomsCount(6);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 22 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 14 );
    Room dining    = roomBuilder.createRoom("Dining" , 15 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bath3");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Bedroom3");
    templateDesign.addConnection("Corridor" , "Bedroom4");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate11()
{
    Design templateDesign;

    templateDesign.setArea(179);
    templateDesign.setBedroomsCount(4);
    templateDesign.setRoomsCount(7);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 23 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 15 );
    Room dining    = roomBuilder.createRoom("Dining" , 16 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bath3");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Bedroom3");
    templateDesign.addConnection("Corridor" , "Bedroom4");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Family");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");

    return templateDesign;
}

Design TemplateRepository::createTemplate12()
{
    Design templateDesign;

    templateDesign.setArea(206);
    templateDesign.setBedroomsCount(5);
    templateDesign.setRoomsCount(8);

    Room corridor  = roomBuilder.createRoom("Corridor" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 24 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 16 );
    Room dining    = roomBuilder.createRoom("Dining" , 17 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );

    templateDesign.addRoom(corridor);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);

    templateDesign.addConnection("Corridor" , "Bath1");
    templateDesign.addConnection("Corridor" , "Bath2");
    templateDesign.addConnection("Corridor" , "Bath3");
    templateDesign.addConnection("Corridor" , "Bedroom1");
    templateDesign.addConnection("Corridor" , "Bedroom2");
    templateDesign.addConnection("Corridor" , "Bedroom3");
    templateDesign.addConnection("Corridor" , "Bedroom4");
    templateDesign.addConnection("Corridor" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Family");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Family"   , "Office");

    return templateDesign;
}

Design TemplateRepository::createTemplate13()
{
    Design templateDesign;

    templateDesign.setArea(215);
    templateDesign.setBedroomsCount(6);
    templateDesign.setRoomsCount(9);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 25 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 17 );
    Room dining    = roomBuilder.createRoom("Dining" , 18 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom    = roomBuilder.createRoom("PlayRoom" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");

    return templateDesign;
}

Design TemplateRepository::createTemplate14()
{
    Design templateDesign;

    templateDesign.setArea(238);
    templateDesign.setBedroomsCount(6);
    templateDesign.setRoomsCount(9);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 26 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 18 );
    Room dining    = roomBuilder.createRoom("Dining" , 19 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom    = roomBuilder.createRoom("PlayRoom" , -1 );
    Room bath4    = roomBuilder.createRoom("Bath4" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);
    templateDesign.addRoom(bath4);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");
    templateDesign.addConnection("Corridor2"  , "Bath4");

    return templateDesign;
}

Design TemplateRepository::createTemplate15()
{
    Design templateDesign;

    templateDesign.setArea(261);
    templateDesign.setBedroomsCount(7);
    templateDesign.setRoomsCount(10);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 27 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 19 );
    Room dining    = roomBuilder.createRoom("Dining" , 20 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom    = roomBuilder.createRoom("PlayRoom" , -1 );
    Room bath4    = roomBuilder.createRoom("Bath4" , -1 );
    Room multiPurpose    = roomBuilder.createRoom("MultiPurpose" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);
    templateDesign.addRoom(bath4);
    templateDesign.addRoom(multiPurpose);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");
    templateDesign.addConnection("Corridor2"  , "Bath4");
    templateDesign.addConnection("Corridor2"  , "MultiPurpose");

    return templateDesign;
}

Design TemplateRepository::createTemplate16()
{
    Design templateDesign;

    templateDesign.setArea(280);
    templateDesign.setBedroomsCount(8);
    templateDesign.setRoomsCount(11);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 28 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 20 );
    Room dining    = roomBuilder.createRoom("Dining" , 21 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom  = roomBuilder.createRoom("PlayRoom" , -1 );
    Room bath4     = roomBuilder.createRoom("Bath4" , -1 );
    Room multiPurpose    = roomBuilder.createRoom("MultiPurpose" , -1 );
    Room sunRoom    = roomBuilder.createRoom("SunRoom" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);
    templateDesign.addRoom(bath4);
    templateDesign.addRoom(multiPurpose);
    templateDesign.addRoom(sunRoom);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");
    templateDesign.addConnection("Corridor2"  , "Bath4");
    templateDesign.addConnection("Corridor2"  , "MultiPurpose");
    templateDesign.addConnection("Corridor2"  , "SunRoom");

    return templateDesign;
}

Design TemplateRepository::createTemplate17()
{
    Design templateDesign;

    templateDesign.setArea(289);
    templateDesign.setBedroomsCount(8);
    templateDesign.setRoomsCount(11);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 29 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 21 );
    Room dining    = roomBuilder.createRoom("Dining" , 22 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom    = roomBuilder.createRoom("PlayRoom" , -1 );
    Room bath4    = roomBuilder.createRoom("Bath4" , -1 );
    Room bath5    = roomBuilder.createRoom("Bath5" , -1 );
    Room multiPurpose    = roomBuilder.createRoom("MultiPurpose" , -1 );
    Room sunRoom    = roomBuilder.createRoom("SunRoom" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);
    templateDesign.addRoom(bath4);
    templateDesign.addRoom(bath5);
    templateDesign.addRoom(multiPurpose);
    templateDesign.addRoom(sunRoom);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");
    templateDesign.addConnection("Corridor2"  , "Bath4");
    templateDesign.addConnection("Corridor2"  , "Bath5");
    templateDesign.addConnection("Corridor2"  , "MultiPurpose");
    templateDesign.addConnection("Corridor2"  , "SunRoom");

    return templateDesign;
}

Design TemplateRepository::createTemplate18()
{
    Design templateDesign;

    templateDesign.setArea(309);
    templateDesign.setBedroomsCount(9);
    templateDesign.setRoomsCount(12);

    Room corridor1  = roomBuilder.createRoom("Corridor1" , 18 );
    Room corridor2  = roomBuilder.createRoom("Corridor2" , 18 );
    Room bedroom1  = roomBuilder.createRoom("Bedroom1" , -1 );
    Room bedroom2  = roomBuilder.createRoom("Bedroom2" , -1 );
    Room bedroom3  = roomBuilder.createRoom("Bedroom3" , -1 );
    Room bedroom4  = roomBuilder.createRoom("Bedroom4" , -1 );
    Room bath1     = roomBuilder.createRoom("Bath1" , -1 );
    Room bath2     = roomBuilder.createRoom("Bath2" , -1 );
    Room bath3     = roomBuilder.createRoom("Bath3" , -1 );
    Room living    = roomBuilder.createRoom("Living" , 30 );
    Room kitchen   = roomBuilder.createRoom("Kitchen" , 22 );
    Room dining    = roomBuilder.createRoom("Dining" , 23 );
    Room gp        = roomBuilder.createRoom("GB" , 4 );
    Room family    = roomBuilder.createRoom("Family" , -1 );
    Room office    = roomBuilder.createRoom("Office" , -1 );
    Room playRoom    = roomBuilder.createRoom("PlayRoom" , -1 );
    Room bath4    = roomBuilder.createRoom("Bath4" , -1 );
    Room bath5    = roomBuilder.createRoom("Bath5" , -1 );
    Room multiPurpose    = roomBuilder.createRoom("MultiPurpose" , -1 );
    Room sunRoom    = roomBuilder.createRoom("SunRoom" , -1 );
    Room homeTheater    = roomBuilder.createRoom("HomeTheater" , -1 );

    templateDesign.addRoom(corridor1);
    templateDesign.addRoom(corridor2);
    templateDesign.addRoom(bedroom1);
    templateDesign.addRoom(bedroom2);
    templateDesign.addRoom(bedroom3);
    templateDesign.addRoom(bedroom4);
    templateDesign.addRoom(bath1);
    templateDesign.addRoom(bath2);
    templateDesign.addRoom(bath3);
    templateDesign.addRoom(living);
    templateDesign.addRoom(kitchen);
    templateDesign.addRoom(gp);
    templateDesign.addRoom(dining);
    templateDesign.addRoom(family);
    templateDesign.addRoom(office);
    templateDesign.addRoom(playRoom);
    templateDesign.addRoom(bath4);
    templateDesign.addRoom(bath5);
    templateDesign.addRoom(multiPurpose);
    templateDesign.addRoom(sunRoom);
    templateDesign.addRoom(homeTheater);

    templateDesign.addConnection("Corridor1" , "Bath1");
    templateDesign.addConnection("Corridor1" , "Bath2");
    templateDesign.addConnection("Corridor1" , "Bath3");
    templateDesign.addConnection("Corridor1" , "Bedroom1");
    templateDesign.addConnection("Corridor1" , "Bedroom2");
    templateDesign.addConnection("Corridor1" , "Bedroom3");
    templateDesign.addConnection("Corridor1" , "Bedroom4");
    templateDesign.addConnection("Corridor1" , "Living");
    templateDesign.addConnection("Living"   , "Dining");
    templateDesign.addConnection("Living"   , "Corridor2");
    templateDesign.addConnection("Dining"   , "GB");
    templateDesign.addConnection("Dining"   , "Kitchen");
    templateDesign.addConnection("Corridor2"  , "Family");
    templateDesign.addConnection("Corridor2"  , "Office");
    templateDesign.addConnection("Corridor2"  , "PlayRoom");
    templateDesign.addConnection("Corridor2"  , "Bath4");
    templateDesign.addConnection("Corridor2"  , "Bath5");
    templateDesign.addConnection("Corridor2"  , "MultiPurpose");
    templateDesign.addConnection("Corridor2"  , "SunRoom");
    templateDesign.addConnection("Corridor2"  , "HomeTheater");

    return templateDesign;
}
