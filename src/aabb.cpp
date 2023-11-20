#include "aabb.h"

#include <algorithm>
#include <limits>

namespace RayTracer {

AABB::AABB() {
    min_ = Vector3(0,0,0);
    max_ = Vector3(0,0,0);
}

AABB::AABB(Vector3 position, float radius) {
    min_ = position - Vector3(radius, radius, radius);
    max_ = position + Vector3(radius, radius, radius);
}

AABB::AABB(Vector3 v1, Vector3 v2, Vector3 v3) {
    min_ = Vector3::Min(v1, Vector3::Min(v2, v3));
    max_ = Vector3::Max(v1, Vector3::Max(v2, v3));
}

AABB::AABB(Vector3 min, Vector3 max) {
    min_ = min;
    max_ = max;
}

bool AABB::IntersectsRay(Ray ray) const {
    // Check easy cases
    if (ray.Direction().x() == 0 && (ray.Origin().x() < min_.x() || ray.Origin().x() > max_.x()))
        return false;
    if (ray.Direction().y() == 0 && (ray.Origin().y() < min_.y() || ray.Origin().y() > max_.y()))
        return false;
    if (ray.Direction().z() == 0 && (ray.Origin().z() < min_.z() || ray.Origin().z() > max_.z()))
        return false;

    Vector3 inverseRayDirection = 1 / ray.Direction();

    float tx1 = (min_.x() - ray.Origin().x())*inverseRayDirection.x();
    float tx2 = (max_.x() - ray.Origin().x())*inverseRayDirection.x();

    float ty1 = (min_.y() - ray.Origin().y())*inverseRayDirection.y();
    float ty2 = (max_.y() - ray.Origin().y())*inverseRayDirection.y();

    float tz1 = (min_.z() - ray.Origin().z())*inverseRayDirection.z();
    float tz2 = (max_.z() - ray.Origin().z())*inverseRayDirection.z();

    float tStart = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2));
    float tEnd = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));

    if (tStart > tEnd || tEnd < 0)
        return false;

    return true;
}

}  // namespace RayTracer
