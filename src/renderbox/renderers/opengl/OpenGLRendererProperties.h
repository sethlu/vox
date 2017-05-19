#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include "../../objects/Mesh.h"
#include "OpenGLObjectProperties.h"


#ifndef PARTICLESIM_OPENGLPROPERTIES_H
#define PARTICLESIM_OPENGLPROPERTIES_H


namespace renderbox {

    class OpenGLRendererProperties {
        std::unordered_map<int, OpenGLObjectProperties *> objectProperties;
    public:
        OpenGLObjectProperties *getObjectProperties(Object *object);
    };

}


#endif //PARTICLESIM_OPENGLPROPERTIES_H
