#include "utilities.h"

namespace RayTracer {

std::string Utilities::ReplaceExtension(const std::string& filename, std::string new_extension)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename + new_extension;
    return filename.substr(0, lastdot) + new_extension;
}

std::vector<std::string> Utilities::SplitString(std::string s, std::string del)
{
    std::vector<std::string> splitString;
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        splitString.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    splitString.push_back(s.substr(start, end - start));
    return splitString;
}

}  // namespace RayTracer
