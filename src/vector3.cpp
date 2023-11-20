#include "vector3.h"

#include <math.h>
#include <limits>
#include <algorithm>

namespace RayTracer {

Vector3::Vector3() {
    x_ = 0;
    y_ = 0;
    z_ = 0;
}

Vector3::Vector3(float x, float y, float z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

float Vector3::Length() const {
    return sqrt(x_*x_ + y_*y_ + z_*z_);
}

void Vector3::Normalize() {
    float length = Length();
    if (length < std::numeric_limits<float>::epsilon()) return;
    x_ /= length;
    y_ /= length;
    z_ /= length;
}

Vector3 Vector3::Normalize(const Vector3& v) {
    float length = v.Length();
    if (length < std::numeric_limits<float>::epsilon()) return v;
    return v / length;
}

float Vector3::Distance(const Vector3& v1, const Vector3& v2) {
    return sqrt(
        (v1.x_-v2.x_)*(v1.x_-v2.x_) +
        (v1.y_-v2.y_)*(v1.y_-v2.y_) +
        (v1.z_-v2.z_)*(v1.z_-v2.z_)
    );
}

float Vector3::Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x_*v2.x_ + v1.y_*v2.y_ + v1.z_*v2.z_;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        v1.y_*v2.z_-v1.z_*v2.y_,
        v1.z_*v2.x_-v1.x_*v2.z_,
        v1.x_*v2.y_-v1.y_*v2.x_
    );
}

Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        std::max(v1.x_, v2.x_),
        std::max(v1.y_, v2.y_),
        std::max(v1.z_, v2.z_)
    );
}

Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        std::min(v1.x_, v2.x_),
        std::min(v1.y_, v2.y_),
        std::min(v1.z_, v2.z_)
    );
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

Vector3 operator-(const Vector3& v) {
    return Vector3(-v.x(), -v.y(), -v.z());
}

Vector3 operator*(const float s, const Vector3& v) {
    return Vector3(s * v.x(), s * v.y(), s * v.z());
}

Vector3 operator*(const Vector3& v, const float s) {
    return Vector3(s * v.x(), s * v.y(), s * v.z());
}

Vector3 operator*(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

Vector3 operator/(const Vector3& v, const float s) {
    return Vector3(v.x() / s, v.y() / s, v.z() / s);
}

Vector3 operator/(const float s, const Vector3& v) {
    return Vector3(s / v.x(), s / v.y(), s / v.z());
}

}  // namespace RayTracer
