#include "image.h"
#include "utilities.h"

#include <vector>
#include <sstream>
    #include <iostream>

namespace RayTracer {

Image::Image() {
    width_ = 240;
    height_ = 240;
    pixels_ = new Color[width_*height_];
}

Image::Image(int width, int height) {
    width_ = width;
    height_ = height;
    pixels_ = new Color[width_*height_];
}

Image::Image(int width, int height, Color* pixels) {
    width_ = width;
    height_ = height;
    pixels_ = pixels;
}

Image::~Image() {
    delete pixels_;
}

int Image::Idx(int x, int y) const {
    return x + y*width_;
}
Color Image::GetPixel(int x, int y) const {
    return pixels_[Idx(x, y)];
}

Color Image::GetPixel(float u, float v) const {
    int x = (int) (u*(width_-1));
    int y = (int) (v*(height_-1));
    return pixels_[Idx(x, y)];
}

void Image::SetPixel(int x, int y, Color color) {
    pixels_[Idx(x, y)] = color;
}

void Image::WriteToPPMFile(std::string outputFileName) {
    // Open output file for writing
    std::ofstream outputStream(outputFileName, std::ios::out);
    // Write the header to the output file
    outputStream << "P3\n"
        << width_ << " " << height_ << "\n"
        << 255 << "\n";

    // Write the pixels, starting at the top left corner of the image
    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            Color c = GetPixel(x, y);
            outputStream << (int)(c.r()*255) << " " << (int)(c.g()*255) << " " << (int)(c.b()*255) << "\n";
        }
    }

    // Close the output file
    outputStream.close();
}

Image* Image::ReadPPM(std::string ppmImageFileName) {
    std::ifstream ppmImageFile;
    ppmImageFile.open(ppmImageFileName);
    if (!ppmImageFile) {
        throw std::invalid_argument("Invalid file name.");
    }
    int i = 0;
    int pixelIdx = 0;
    bool header = true;
    std::string line;
    float r, g, b;
    float norm;
    int height;
    int width;
    Color* pixels;
    while (std::getline(ppmImageFile, line)) {
        std::vector<std::string> values;
        std::istringstream iss(line);
    	for (std::string s; iss >> s; )
    		values.push_back(s);
        if (header) {
            width = std::stoi(values[1]);
            height = std::stoi(values[2]);
            norm = std::stof(values[3]);
            pixels = new Color[width*height];
            header = false;
        } else {
            for (auto value : values) {
                if (i == 0) {
                    r = std::stof(value) / norm;
                } else if (i == 1) {
                    g = std::stof(value) / norm;
                } else if (i == 2) {
                    b = std::stof(value) / norm;
                    pixels[pixelIdx++] = Color(r, g, b);
                }
                i = (i + 1) % 3;
            }
        }
    }
    return new Image(width, height, pixels);
}

}  // namespace RayTracer
