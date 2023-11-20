#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <vector>

namespace RayTracer {

/// A class containing static helper functions.
class Utilities {
public:
    // Replaces a file extention with the given string (where given string should include ".")
    static std::string ReplaceExtension(const std::string& filename, std::string new_extension);
    // Splits string by a delimeter
    static std::vector<std::string> SplitString(std::string s, std::string del = " ");
};

}  // namespace RayTracer

#endif  // UTILITIES_H_
