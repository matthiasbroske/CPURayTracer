#include "camera.h"

namespace RayTracer {

Camera::Camera() {
    eyePosition_ = Vector3(0, 0, 0);
    viewDirection_ = Vector3(0, 0, -1);
    upDirection_ = Vector3(0, 1, 0);
    fieldOfView_ = 45;
    width_ = 240;
    height_ = 240;
}

Camera::Camera(Vector3 eyePosition, Vector3 viewDirection, Vector3 upDirection, float fieldOfView, int width, int height) {
    eyePosition_ = eyePosition;
    viewDirection_ = Vector3::Normalize(viewDirection);
    upDirection_ = Vector3::Normalize(upDirection);
    fieldOfView_ = fieldOfView;
    width_ = width;
    height_ = height;
}

}  // namespace RayTracer
