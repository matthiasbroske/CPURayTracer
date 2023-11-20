#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <ctime>

#include "scene.h"
#include "vector3.h"
#include "image.h"
#include "utilities.h"

using namespace RayTracer;

int main(int argc, char **argv) {
    // Supply the user with usage info if they did not enter enough command line arguments
    if (argc < 2) {
        std::cout << "usage: scenefile [outputfile] [softshadows] [dof]\n"
            << "scenefile - path to input file containing scene description\n"
            << "outputfile - name for final output image file (optional)\n"
            << "softshadows -  soft shadow toggle, 0 = off, 1 = on (optional)\n"
            << "dof - depth of field toggle, 0 = off, 1 = on (optional)\n";
        return -1;
    }

    // Get the scene and output file names from the command line arguments and ensure they are valid
    std::string sceneFileName = argv[1];
    std::string outputFileName = argc > 2 ?
        Utilities::ReplaceExtension(argv[2], ".ppm") :
        Utilities::ReplaceExtension(sceneFileName, ".ppm");
    std::ifstream sceneFile;
    sceneFile.open(sceneFileName);
    if (!sceneFile) {
        std::cout << "Scene file does not exist. Please try again.\n";
        return -1;
    }
    bool softShadows = false;
    if (argc > 3) {
        try {
            softShadows = std::stoi(argv[3]) == 0 ? false : true;
        } catch (std::invalid_argument& e) {
            std::cout << "Soft shadows flag not specified correctly.\n";
            return -1;
        }
    }
    bool depthOfField = false;
    if (argc > 4) {
        try {
            depthOfField = std::stoi(argv[4]) == 0 ? false : true;
        } catch (std::invalid_argument& e) {
            std::cout << "Depth of field not specified correctly.\n";
            return -1;
        }
    }

    // Try to initialize a scene using the scene file
    Scene* scene = new Scene(softShadows, depthOfField);
    SceneInitStatus sceneInitStatus = scene->InitFromFile(sceneFile);
    // Print an error message specifying what went wrong if unsuccessful
    if (sceneInitStatus != Success) {
        std::cout << "Scene file load error: " << sceneInitStatusText[sceneInitStatus] << " is not properly defined.\n";
        delete scene;
        return -1;
    }
    // Construct a BVH for the scene to improve ray tracing speed
    scene->ConstructBVH();

    // Render a ray traced image of the scene
    Image renderImage = scene->Render();

    // Write the rendered image to an output file for viewing
    renderImage.WriteToPPMFile(outputFileName);

    // Delete scene and exit
    delete scene;
    return 0;
}
