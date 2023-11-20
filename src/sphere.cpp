#include "sphere.h"

#include <limits>
#include <cmath>

namespace RayTracer {

Sphere::Sphere() : SceneObject() {
    radius_ = 1.0;
}

Sphere::Sphere(Vector3 position, float radius, int materialIdx, int textureIdx) : SceneObject(position, materialIdx, textureIdx) {
    radius_ = radius;
}

AABB Sphere::BoundingBox() const {
    return AABB(position_, radius_);
}

RaycastHit Sphere::IntersectRay(Ray ray) const {
    // Initialize raycast hit info
    RaycastHit hitInfo;
    hitInfo.hit = false;
    hitInfo.distance = std::numeric_limits<float>::infinity();

    // Find the point closest to the center of the sphere
    float t = Vector3::Dot(position_-ray.Origin(), ray.Direction());  // Distance from ray origin to point closest to the center of the sphere
    Vector3 p = ray.Origin() + t*ray.Direction();  // Point closest to the center of the sphere
    float y = (position_-p).Length();  // Distance from point to center of sphere
    // If the point is within the sphere, then the ray intersects:
    if (y < radius_) {
        float x = sqrt(radius_*radius_ - y*y);  // Distance from t along ray to intersection points
        float t1 = t - x;  // Closest intersection distance
        //float t2 = t + x;  // Farthest intersection distance
        // Only consider it an intersection if it's in the positive direction along the ray
        if (t1 > 0) {
            hitInfo.hit = true;
            hitInfo.distance = t1;
            hitInfo.point = ray.GetPoint(t1);
            hitInfo.normal = Vector3::Normalize(hitInfo.point - position_);
            hitInfo.materialIdx = materialIdx_;
            hitInfo.textureIdx = textureIdx_;
            if (textureIdx_ != -1) {
                float theta = std::atan2(hitInfo.normal.x(), hitInfo.normal.z());
                float phi = std::acos(hitInfo.normal.y());
                hitInfo.u = (theta + M_PI) / (2*M_PI);//theta > 0 ? theta/(2*M_PI) : (theta + 2*M_PI) / (2*M_PI);
                hitInfo.v = phi / M_PI;
            }
            hitInfo.object = this;
        }
    }

    // Return infinity if no collision
    return hitInfo;
}

}  // namespace RayTracer
