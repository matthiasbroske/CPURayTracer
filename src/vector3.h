#ifndef VECTOR3_H_
#define VECTOR3_H_

namespace RayTracer {

/// A 3D vector. Allows for basic mathematical operations and can
/// be used to represent both points and true "vectors".
class Vector3 {
public:

    /// Default constructor creates a zero vector
    Vector3();
    /// Creates a vector (x, y, z)
    Vector3(float x, float y, float z);

    /// Getters for x, y and z vector components
    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }

    /// Returns the length of the vector
    float Length() const;

    /// Normalizes the vector
    void Normalize();

    /// Returns normalized vector
    static Vector3 Normalize(const Vector3& v);
    /// Returns distance between two points
    static float Distance(const Vector3& v1, const Vector3& v2);
    /// Returns the dot product of two vectors
    static float Dot(const Vector3& v1, const Vector3& v2);
    /// Returns the cross product of two vectors
    static Vector3 Cross(const Vector3& v1, const Vector3& v2);
    /// Returns component by component max of two vectors
    static Vector3 Max(const Vector3& v1, const Vector3& v2);
    /// Returns component by component min of two vectors
    static Vector3 Min(const Vector3& v1, const Vector3& v2);

private:
    float x_, y_, z_;
};

// ----- Operator Overloads -----

/// Adds two vectors
Vector3 operator+(const Vector3& v1, const Vector3& v2);

/// Subtracts two vectors
Vector3 operator-(const Vector3& v1, const Vector3& v2);
/// Negates a vector
Vector3 operator-(const Vector3& v1);

/// Multiplies a vector by a scalar
Vector3 operator*(const float s, const Vector3& v);
Vector3 operator*(const Vector3& v, const float s);

// Multiplies a vector by a vector (component-wise)
Vector3 operator*(const Vector3& v1, const Vector3& v2);

/// Divides a vector by a scalar
Vector3 operator/(const Vector3& v, const float s);
Vector3 operator/(const float s, const Vector3& v);

}  // namespace RayTracer

#endif  // VECTOR3_H_
