#ifndef SCENE_H_
#define SCENE_H_

#include "camera.h"
#include "color.h"
#include "scene_object.h"
#include "point_light.h"
#include "directional_light.h"
#include "material.h"
#include "image.h"
#include "bvh_node.h"

#include <vector>
#include <fstream>
#include <string>

namespace RayTracer {

#define SHADOW_SAMPLE_COUNT 50
#define MAX_DEPTH 8
#define IOR_AIR 1
#define DOF_SAMPLE_COUNT 20

/// Scene init errors and their corresponding status text
enum SceneInitStatus {
    Success,
    EyeError,
    ViewDirError,
    UpDirError,
    VFovError,
    ImSizeError,
    BkgColorError,
    MtlColorError,
    SphereError,
    CylinderError,
    LightError,
    DepthCueingError,
    VertexError,
    NormalError,
    TexCoordError,
    TriangleError,
    TextureError
};
const std::string sceneInitStatusText[] = {
    "success",
    "eye",
    "viewdir",
    "updir",
    "vfov",
    "imsize",
    "bkgcolor",
    "mtlcolor",
    "sphere",
    "cylinder",
    "light",
    "depthcueing",
    "v",
    "vn",
    "vt",
    "f",
    "texture"
};

/// A scene containing objects and a camera to render them.
class Scene {
public:

    /// Default constructor creates an empty scene with a default camera
    Scene(bool softShadows = false, bool depthOfField = false);
    /// Creates a scene with given camera and background color
    Scene(Camera camera, Color backgroundColor, bool softShadows = false, bool depthOfField = false);
    /// Initializes the scene from a scene description file
    SceneInitStatus InitFromFile(std::ifstream& sceneFile);
    /// Deletes all objects in scene
    ~Scene();

    /// Adds an object to the scene
    void AddObjectToScene(SceneObject* sceneObject);
    /// Adds a point light to the scene
    void AddPointLightToScene(PointLight pointLight);
    /// Adds a directional light to the scene
    void AddDirectionalLightToScene(DirectionalLight directionalLight);

    /// Getters
    Camera SceneCamera() const { return camera_; }
    Color BackgroundColor() const { return backgroundColor_; }
    Color DepthCueingColor() const { return depthCueingColor_; }
    std::vector<PointLight> PointLights() const { return pointLights_; }
    std::vector<DirectionalLight> DirectionalLights() const { return directionalLights_; }

    /// Constructs a BVH for the objects currently in the scene
    void ConstructBVH();
    /// Returns an image of the scene rendered by tracing rays for each pixel
    Image Render();
    /// Returns the color of a ray traced into the scene
    Color TraceRay(const Ray ray, int iteration = 0, const SceneObject* ignoreObject = NULL) const;
    /// Casts a ray into the scene, returning info about the nearest hit
    RaycastHit Raycast(const Ray ray, const SceneObject* ignoreObject = NULL) const;

private:
    Vector3 ComputeDiffuseSpecular(Vector3 L, Vector3 N, Vector3 V, Vector3 Od, Vector3 Os, float ka, float kd, float ks, float n) const;
    float InShadow(Vector3 point, Vector3 lightPosition, const SceneObject* ignoreObject = NULL) const;
    Color DepthCue(Vector3 I, float d) const;
    BVHNode* ConstructBVHRecursive(std::vector<SceneObject*>& sceneObjects);
    RaycastHit RaycastBVH(const Ray ray, BVHNode* node, const SceneObject* ignoreObject) const;
    static bool CompareObectX(SceneObject* a, SceneObject* b) { return a->Position().x() < b->Position().x(); }
    static bool CompareObectY(SceneObject* a, SceneObject* b) { return a->Position().y() < b->Position().y(); }
    static bool CompareObectZ(SceneObject* a, SceneObject* b) { return a->Position().z() < b->Position().z(); }
    float viewingDistance_ = 3;
    bool softShadows_ = false;
    bool depthOfField_ = false;
    Camera camera_;
    Color backgroundColor_;
    Color depthCueingColor_;
    float aMax_, aMin_, distMax_, distMin_;
    std::vector<Material> materials_;
    std::vector<Image*> textures_;
    std::vector<SceneObject*> sceneObjects_;
    std::vector<PointLight> pointLights_;
    std::vector<DirectionalLight> directionalLights_;
    BVHNode* bvhRoot_;
};

}

#endif  // SCENE_H_
