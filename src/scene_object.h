#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#include "vector3.h"
#include "material.h"
#include "ray.h"
#include "raycast_hit.h"
#include "aabb.h"

namespace RayTracer {

class AABB;

/// An object that can be positioned in the scene, rendered, and intersected with.
class SceneObject {
public:

    /// Default constructor creates a scene object cenetered at the origin
    SceneObject();
    /// Creates a scene object at a given position with specified material and texture
    SceneObject(Vector3 position, int materialIdx, int textureIdx);
    /// Destructor
    virtual ~SceneObject() {}

    /// Returns object position
    Vector3 Position() const { return position_; }
    /// Returns object bounding box
    virtual AABB BoundingBox() const;

    /// Performs a raycast against this object, returning raycast hit information
    virtual RaycastHit IntersectRay(Ray ray) const;

protected:
    Vector3 position_;
    int materialIdx_;
    int textureIdx_;
};

}  // namespace RayTracer

#endif  // SCENE_OBJECT_H_
