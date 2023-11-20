#ifndef POINT_LIGHT_H_
#define POINT_LIGHT_H_

#include "vector3.h"
#include "color.h"

namespace RayTracer {

/// A point light defined by its position, color
/// and optional attenuation factors.
class PointLight {
public:

    /// Default constructor creates a white point light at the origin
    PointLight();
    /// Creates a point light with specified position and color
    PointLight(Vector3 position, Color color);
    /// Creates a point light with attenuation
    PointLight(Vector3 position, Color color, float c1, float c2, float c3);
    /// Destructor
    ~PointLight() {}

    /// Returns light attenuation factor from light to given point,
    /// returning 1 if the light does not support attenuation.
    float Attenuate(Vector3 point) const;

    /// Returns the light position
    Vector3 Position() const { return position_; }
    /// Returns the light color
    Color LightColor() const { return color_; }

private:
    Vector3 position_;
    Color color_;
    bool attenuated_;
    float c1_;
    float c2_;
    float c3_;
};

}  // namespace RayTracer

#endif  // POINT_LIGHT_H_
