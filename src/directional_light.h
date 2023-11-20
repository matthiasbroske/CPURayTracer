#ifndef DIRECTIONAL_LIGHT_H_
#define DIRECTIONAL_LIGHT_H_

#include "vector3.h"
#include "color.h"

namespace RayTracer {

/// A directional light defined by its direction and color.
class DirectionalLight {
public:

    /// Default constructor creates a white directional light pointing down
    DirectionalLight();
    /// Creates a directional light with specified direction and color
    DirectionalLight(Vector3 direction, Color color);
    /// Destructor
    ~DirectionalLight() {}

    /// Returns the light direction
    Vector3 Direction() const { return direction_; }
    /// Returns the light color
    Color LightColor() const { return color_; }

private:
    Vector3 direction_;
    Color color_;
};

}  // namespace RayTracer

#endif  // DIRECTIONAL_LIGHT_H_
