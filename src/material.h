#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "color.h"

namespace RayTracer {

/// A struct containing basic material properties
/// used in the Phong lighting model.
struct Material {
    Color Od, Os;
    float ka, kd, ks, n;
    float a, ior;
};

}  // namespace RayTracer

#endif  // MATERIAL_H_
