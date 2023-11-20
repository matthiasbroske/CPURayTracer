#ifndef RAYCAST_HIT_H_
#define RAYCAST_HIT_H_

#include "vector3.h"
#include "scene_object.h"

#include <limits>

namespace RayTracer {

class SceneObject;

/// A struct containing raycast hit information, such as
/// distance, normal direction, material reference, texture reference, etc.
struct RaycastHit {
    bool hit = false;
    float distance = std::numeric_limits<float>::infinity();
    Vector3 point;
    Vector3 normal;
    int materialIdx;
    int textureIdx;
    float u, v;
    const SceneObject* object;
};

}  // namespace RayTracer

#endif  // RAY_H_
