#ifndef RENDERBOX_OBJECT_H
#define RENDERBOX_OBJECT_H


#include <vector>

#include "Geometry.h"
#include "Material.h"
#include "Matrix.h"
#include "VersionTrackedObject.h"

namespace renderbox {

    enum OBJECT_TYPE {
        GROUP_OBJECT  = 0x0,
        MESH_OBJECT   = 0x1,
        SCENE_OBJECT  = 0x2,
        CAMERA_OBJECT = 0x4,
        LIGHT_OBJECT  = 0x8,
        BONE_OBJECT   = 0x10,
    };

    class Object : public VersionTrackedObject {
    public:

        typedef vec3 translation_type;

        explicit Object(std::shared_ptr<Geometry> geometry = nullptr, std::shared_ptr<Material> material = nullptr);

        virtual ~Object();

        int getObjectId() const;

        virtual OBJECT_TYPE getObjectType() const {
            return GROUP_OBJECT;
        }

        bool isMesh() const {
            return (getObjectType() & MESH_OBJECT) != 0;
        }

        bool isLight() const {
            return (getObjectType() & LIGHT_OBJECT) != 0;
        }

        /**
         * Returns a shallow copy of the object
         */
        std::shared_ptr<Object> clone();

        // Object inheritance

        bool hasParent() const;

        /**
         * @var Parent of the object
         */
        Object *parent = nullptr;

        /**
         * @var Children of the object
         */
        std::vector<std::shared_ptr<Object>> children;

        void addChild(std::shared_ptr<Object> child);

        Object *getRoot();

        // Geometry

        bool hasGeometry();

        std::shared_ptr<Geometry> getGeometry();

        void setGeometry(std::shared_ptr<Geometry> const &geometry);

        // Material

        bool hasMaterial();

        std::shared_ptr<Material> getMaterial();

        void setMaterial(std::shared_ptr<Material> const &material);

        // Object transformation

        mat4 getWorldMatrix() const;

        mat4 getMatrix() const;

        vec3 getWorldPosition() const;

        // Translation

        translation_type getTranslation() const;

        void setTranslation(translation_type translation);

        void translate(translation_type delta);

        // Rotation

        mat4 getRotationMatrix() const;

        void rotate(vec3 vector, float angle);

        void clearRotation();

        // Scale

        vec3 getScale() const;

        void setScale(vec3 scale);

        // Visibility

        bool visible = true;

    protected:

        /**
         * @var Object geometry
         */
        std::shared_ptr<Geometry> geometry;

        /**
         * @var Object material
         */
        std::shared_ptr<Material> material;

        // Object internal management

        /**
         * @var Global object count
         * @static
         */
        static int count;

        /**
         * @var Global unique object ID
         */
        int objectId = ++count;

        // Object transformation

        /**
         * @var Object's world matrix
         */
        mat4 worldMatrix = mat4(1.0f);

        /**
         * @var Object translation
         */
        vec3 translation = vec3(0);

        /**
         * @var Object rotation matrix
         */
        mat4 rotationMatrix = mat4(1.0f);

        /**
         * @var Object scaling
         */
        vec3 scale = vec3(1.0f);

        virtual void didTransform();

    };

}


#endif //RENDERBOX_OBJECT_H
