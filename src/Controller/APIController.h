//
// Created by ahmed Ibrahim on 11-Feb-25.
//

#ifndef DESIGNI_BACKEND_APICONTROLLER_H
#define DESIGNI_BACKEND_APICONTROLLER_H
#include "HomeDesignController.h"
#include "src/Model/Template/TemplateDesigns.h"
class APIController {
private :
    HomeDesignController homeDesignController;

    /**
     * For Model 1
     */
    TemplateDesigns model1Templates;

    /**
     * 18 templates designs
     */
    DesignRepository templatesDesigns;
public:
    explicit APIController(crow::SimpleApp& app);

     void templateRoutes(crow::SimpleApp& app);
    void landDivisionRoutes(crow::SimpleApp& app);
     void requestFramingRoutes(crow::SimpleApp& app);
     void rotateDesignRoutes(crow::SimpleApp& app);
     void modifyDesignRoutes(crow::SimpleApp& app);
    void requestDesignRoutes(crow::SimpleApp& app);
    void requestDesign2Routes(crow::SimpleApp& app);

    void preProcessing();
};


#endif //DESIGNI_BACKEND_APICONTROLLER_H
