#include "ray.h"

namespace RayTracer {

Ray::Ray() {
    origin_ = Vector3(0, 0, 0);
    direction_ = Vector3(0, 1, 0);
}

Ray::Ray(Vector3 origin, Vector3 direction) {
    origin_ = origin;
    direction_ = Vector3::Normalize(direction);;
}

Vector3 Ray::GetPoint(float distance) const {
    return origin_ + direction_*distance;
}

}  // namespace RayTracer
