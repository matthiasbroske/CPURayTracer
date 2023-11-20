#ifndef RAY_H_
#define RAY_H_

#include "vector3.h"

namespace RayTracer {

/// A 3D ray defined by its origin and unit direction.
class Ray {
public:

    /// Default constructor creates a ray pointing up from the origin
    Ray();
    /// Creates a ray with specified origin and direction (direction will be normalized)
    Ray(Vector3 origin, Vector3 direction);

    /// Returns the ray origin
    Vector3 Origin() const { return origin_; }
    /// Returns the ray direction
    Vector3 Direction() const { return direction_; }

    /// Returns a point at given distance along the ray
    Vector3 GetPoint(float distance) const;

private:
    Vector3 origin_;
    Vector3 direction_;
};

}  // namespace RayTracer

#endif  // RAY_H_
