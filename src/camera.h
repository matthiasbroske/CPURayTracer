#ifndef CAMERA_H_
#define CAMERA_H_

#include "vector3.h"

namespace RayTracer {

/// A camera defined by its eye position, view direction, up direction
/// vertical field of view, width and height.
class Camera {
public:

    /// Creates a camera object with default parameters
    Camera();
    /// Creates a camera object with specified parameters
    Camera(Vector3 eyePosition, Vector3 viewDirection, Vector3 upDirection, float fieldOfView, int width, int height);

    /// Getters
    Vector3 EyePosition() const { return eyePosition_; }
    Vector3 ViewDirection() const { return viewDirection_; }
    Vector3 UpDirection() const { return upDirection_; }
    float FieldOfView() const { return fieldOfView_; }
    int Width() const { return width_; }
    int Height() const { return height_; }

private:
    Vector3 eyePosition_;
    Vector3 viewDirection_;
    Vector3 upDirection_;
    float fieldOfView_;
    int width_;
    int height_;
};

}  // namespace RayTracer

#endif  // CAMERA_H_
