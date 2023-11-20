#include "color.h"

namespace RayTracer {

Color::Color() {
    r_ = 0;
    g_ = 0;
    b_ = 0;
}

Color::Color(float r, float g, float b) {
    r_ = r > 1 ? 1 : r;
    g_ = g > 1 ? 1 : g;
    b_ = b > 1 ? 1 : b;
}

void Color::Clamp01() {
    if (r_ < 0) r_ = 0;
    else if (r_ > 1) r_ = 1;
    if (g_ < 0) g_ = 0;
    else if (g_ > 1) g_ = 1;
    if (b_ < 0) b_ = 0;
    else if (b_ > 1) b_ = 1;
}

Color operator+(const Color& c1, const Color& c2) {
    return Color(c1.r() + c2.r(), c1.g() + c2.g(), c1.b() + c2.b());
}

Color operator/(const Color& c, const float s) {
    return Color(c.r() / s, c.g() / s, c.b() / s);
}

}  // namespace RayTracer
