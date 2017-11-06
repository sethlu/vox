#ifndef VOX_CAMERA_H
#define VOX_CAMERA_H


#include <glm/mat4x4.hpp>
#include "Object.h"
#include "Ray.h"

namespace renderbox {

    class Camera : public Object {
    protected:

        glm::mat4x4 viewProjectionMatrix;

        glm::mat4x4 projectionMatrix;

        glm::mat4x4 viewMatrix;

        void didTransform() override;

        void projectionMatrixDidUpdate();

        void viewMatrixDidUpdate();

    public:

        virtual ~Camera() = default;

        glm::mat4x4 getViewProjectionMatrix() const;

        glm::mat4x4 getProjectionMatrix() const;

        glm::mat4x4 getViewMatrix() const;

        virtual Ray *getRay(glm::vec2 coordinates = glm::vec2(0)) const = 0;

    };

}

#endif //VOX_CAMERA_H
