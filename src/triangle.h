#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vector3.h"
#include "material.h"
#include "image.h"
#include "scene_object.h"

namespace RayTracer {

class Triangle : public SceneObject {
public:

    /// Default constructor
    Triangle();
    Triangle(Vector3 vertices[3], Vector3 normals[3], Vector3 texCoords[3], int materialIdx, int textureIdx, bool hasNormals = false, bool hasTexCoords = false);
    ~Triangle() {}

    AABB BoundingBox() const;
    RaycastHit IntersectRay(Ray ray) const;

private:
    Vector3 vertices_[3];
    bool hasNormals_ = false;
    Vector3 normals_[3];
    Vector3 normal_;
    float d_;
    bool hasTexCoords_ = false;
    Vector3 texCoords_[3];
};

}

#endif  // TRIANGLE_H_
