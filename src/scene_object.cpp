#include "scene_object.h"

#include <limits>

namespace RayTracer {

SceneObject::SceneObject() {
    position_ = Vector3(0, 0, 0);
    materialIdx_ = 0;
    textureIdx_ = -1;
}

SceneObject::SceneObject(Vector3 position, int materialIdx, int textureIdx) {
    position_ = position;
    materialIdx_ = materialIdx;
    textureIdx_ = textureIdx;
}

AABB SceneObject::BoundingBox() const {
    return AABB();
}

RaycastHit SceneObject::IntersectRay(Ray ray) const {
    RaycastHit hitInfo;
    return hitInfo;
}


}  // namespace RayTracer
