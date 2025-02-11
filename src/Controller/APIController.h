//
// Created by ahmed Ibrahim on 11-Feb-25.
//

#ifndef DESIGNI_BACKEND_APICONTROLLER_H
#define DESIGNI_BACKEND_APICONTROLLER_H
#include "HomeDesignController.h"
class APIController {
private :
    HomeDesignController homeDesignController;

    /**
     * 18 templates designs
     */
    DesignRepository templatesDesigns;
public:
    explicit APIController(crow::SimpleApp& app);

    static void templateRoutes(crow::SimpleApp& app);
    void landDivisionRoutes(crow::SimpleApp& app);
    static void requestFramingRoutes(crow::SimpleApp& app);
    static void rotateDesignRoutes(crow::SimpleApp& app);
    static void modifyDesignRoutes(crow::SimpleApp& app);
    void requestDesignRoutes(crow::SimpleApp& app);
    void requestDesign2Routes(crow::SimpleApp& app);

    void preProcessing();
};


#endif //DESIGNI_BACKEND_APICONTROLLER_H
