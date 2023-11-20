#include "directional_light.h"

namespace RayTracer {

DirectionalLight::DirectionalLight() {
    direction_ = Vector3(0, -1, 0);
    color_ = Color(1, 1, 1);
}

DirectionalLight::DirectionalLight(Vector3 direction, Color color) {
    direction_ = Vector3::Normalize(direction);
    color_ = color;
}

}  // namespace RayTracer
