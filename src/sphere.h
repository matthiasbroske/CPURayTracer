#ifndef SPHERE_H_
#define SPHERE_H_

#include "scene_object.h"
#include "vector3.h"
#include "raycast_hit.h"

namespace RayTracer {

/// A sphere scene object defined by its position and radius.
class Sphere : public SceneObject {
public:

    /// Default constructor creates a sphere of radius one centered at the origin
    Sphere();
    Sphere(Vector3 position, float radius, int materialIdx, int textureIdx);
    ~Sphere() {}

    /// Returns sphere radius
    float Radius() const { return radius_; }
    // Returns bounding box
    AABB BoundingBox() const;

    /// Performs a raycast against this sphere, returning raycast hit information
    RaycastHit IntersectRay(Ray ray) const;

private:
    float radius_;
};

}  // namespace RayTracer

#endif  // SPHERE_H_
