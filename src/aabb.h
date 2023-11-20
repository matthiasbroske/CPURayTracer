#ifndef AABB_H_
#define AABB_H_

#include "vector3.h"
#include "scene_object.h"
#include "ray.h"

namespace RayTracer {

/// An axis aligned bounding box surrounding a scene object.
class AABB {
public:
    /// Constructs empty bounding box
    AABB();
    /// Constructs AABB for sphere with given position and radius
    AABB(Vector3 position, float radius);
    /// Constructs AABB from triangle with given vertices
    AABB(Vector3 v1, Vector3 v2, Vector3 v3);
    /// Constructs AABB with given min and max
    AABB(Vector3 min, Vector3 max);

    Vector3 Min() const { return min_; }
    Vector3 Max() const { return max_; }

    bool IntersectsRay(Ray ray) const;

private:
    Vector3 min_, max_;
};

}  // namespace RayTracer

#endif  // AABB_H_
