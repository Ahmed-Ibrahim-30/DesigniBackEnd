//
// Created by ahmed Ibrahim on 30-Nov-24.
//

#ifndef DESIGNI_TEMPLATEREPOSITORY_H
#define DESIGNI_TEMPLATEREPOSITORY_H

#include "Design.h"

//class TemplateRepository Responsible for creating 18 Templates
class TemplateRepository {
private:
    static RoomBuilder roomBuilder;
    static Design createTemplate1();
    static Design createTemplate2();
    static Design createTemplate3();
    static Design createTemplate4();
    static Design createTemplate5();
    static Design createTemplate6();
    static Design createTemplate7();
    static Design createTemplate8();
    static Design createTemplate9();
    static Design createTemplate10();
    static Design createTemplate11();
    static Design createTemplate12();
    static Design createTemplate13();
    static Design createTemplate14();
    static Design createTemplate15();
    static Design createTemplate16();
    static Design createTemplate17();
    static Design createTemplate18();
public:
    /**
     * create 18 templates
     */
    static vector<Design> templatesCreations();
};


#endif //DESIGNI_TEMPLATEREPOSITORY_H
