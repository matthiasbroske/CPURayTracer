#ifndef COLOR_H_
#define COLOR_H_

namespace RayTracer {

/// A RGB color defined by floats between 0 and 1.
class Color {
public:

    /// Default constructor creates the color black
    Color();
    /// Creates a color with given (r, g, b)
    Color(float r, float g, float b);

    /// Gets color r, g, and b components
    float r() const { return r_; }
    float g() const { return g_; }
    float b() const { return b_; }

    /// Clamps the color rgb values between 0 and 1
    void Clamp01();

private:
    float r_, g_, b_;
};

Color operator+(const Color& c1, const Color& c2);

Color operator/(const Color& c, const float s);

}  // namespace RayTracer

#endif  // COLOR_H_
