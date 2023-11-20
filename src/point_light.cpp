#include "point_light.h"

namespace RayTracer {

PointLight::PointLight() {
    position_ = Vector3(0, 0, 0);
    color_ = Color(1, 1, 1);
    attenuated_ = false;
}

PointLight::PointLight(Vector3 position, Color color) {
    position_ = position;
    color_ = color;
    attenuated_ = false;
}

PointLight::PointLight(Vector3 position, Color color, float c1, float c2, float c3) {
    position_ = position;
    color_ = color;
    c1_ = c1;
    c2_ = c2;
    c3_ = c3;
    attenuated_ = true;
}

float PointLight::Attenuate(Vector3 point) const {
    if (!attenuated_)
        return 1;

    float d = Vector3::Distance(point, position_);
    float f = 1.0/(c1_ + c2_*d + c3_*d*d);
    return f;
}

}  // namespace RayTracer
