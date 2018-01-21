#ifndef VOX_GEOMETRY_H
#define VOX_GEOMETRY_H


#include "platform.h"
#if defined(__MACOSX__)
#include <OpenGL/OpenGL.h>
#elif defined(__IPHONEOS__)
#include <OpenGLES/ES3/gl.h>
#endif
#include <vector>
#include <glm/vec3.hpp>

namespace renderbox {

    class Geometry {

        friend class OpenGLRenderer;

    public:

        std::vector<glm::vec3> &getVertices();

        std::vector<glm::vec2> &getUVs();

        std::vector<glm::vec3> &getNormals();

        std::vector<glm::uvec3> &getFaces();

    protected:

        std::vector<glm::vec3> vertices;

        std::vector<glm::vec2> uvs;

        std::vector<glm::vec3> normals;

        std::vector<glm::uvec3> faces;

    };

}


#endif //VOX_GEOMETRY_H
