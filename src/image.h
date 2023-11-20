#ifndef IMAGE_H_
#define IMAGE_H_

#include "color.h"

#include <fstream>
#include <string>

namespace RayTracer {

/// A 2D image made up of colored pixels.
class Image {
public:

    /// Default constructor creates a blank 240x240 image
    Image();
    /// Creates an image with given width and height
    Image(int width, int height);
    /// Creates an image with given width and height and heap allocated pixels array
    Image(int width, int height, Color* pixels);
    /// Deletes array of pixels
    ~Image();

    /// Returns image width
    int Width() const { return width_; }
    /// Returns image height
    int Height() const { return height_; }
    /// Gets pixel with (x, y) coordinates (image top left is (0,0))
    Color GetPixel(int x, int y) const;
    /// Gets pixel with (u, v) tex coordinates (image top left is (0,0))
    Color GetPixel(float u, float v) const;
    /// Sets pixel at (x, y)
    void SetPixel(int x, int y, Color color);

    /// Writes image in PPM format to file with given name
    void WriteToPPMFile(std::string outputFileName);

    /// Creates an image from given ppm file
    static Image* ReadPPM(std::string ppmImageFileName);

private:
    inline int Idx(int x, int y) const;
    int width_, height_;
    Color* pixels_;
};

}  // namespace RayTracer

#endif  // IMAGE_H_
