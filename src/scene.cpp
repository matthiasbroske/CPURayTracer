#include "scene.h"
#include "sphere.h"
#include "triangle.h"
#include "ray.h"
#include "utilities.h"

#include <map>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>

namespace RayTracer {

Scene::Scene(bool softShadows, bool depthOfField) {
    camera_ = Camera();
    backgroundColor_ = Color();
    aMax_ = 1;
    aMin_ = 1;
    distMax_ = 1;
    distMin_ = 0;
    softShadows_ = softShadows;
    depthOfField_ = depthOfField;
}

Scene::Scene(Camera camera, Color backgroundColor, bool softShadows, bool depthOfField) {
    camera_ = camera;
    backgroundColor_ = backgroundColor;
    aMax_ = 1;
    aMin_ = 1;
    distMax_ = 1;
    distMin_ = 0;
    softShadows_ = softShadows;
    depthOfField_ = depthOfField;
}

Scene::~Scene() {
    // Delete all objects in scene
    for (size_t i = 0; i < sceneObjects_.size(); i++) {
      delete sceneObjects_[i];
    }
    sceneObjects_.clear();
    // Delete all textures in the scene
    for (size_t i = 0; i < textures_.size(); i++) {
      delete textures_[i];
    }
    textures_.clear();
    // Delete the BVH
    delete bvhRoot_;
}

SceneInitStatus Scene::InitFromFile(std::ifstream& sceneFile) {
    // Map used to parse unique scene file contents (everything but objects and material colors)
    std::map<std::string, std::vector<std::string>> sceneDescription;
    // Map used to parse non-unique scene objects and material colors
    std::vector<std::vector<std::string>> materialDescriptions;
    std::vector<std::pair<std::vector<int>, std::vector<std::string>>> sphereDescriptions;
    // Vector used to parse non-unique scene lights
    std::vector<std::vector<std::string>> lightDescriptions;
    // Vector used to parse textures
    std::vector<std::vector<std::string>> textureDescriptions;
    // Vectors used to parse vertex info
    std::vector<std::vector<std::string>> vertexDescriptions;
    std::vector<std::vector<std::string>> vertexNormalDescriptions;
    std::vector<std::vector<std::string>> texCoordDescriptions;
    // Vector used to parse triangle definitions
    std::vector<std::pair<std::vector<int>, std::vector<std::string>>> triangleDescriptions;

    // Read all lines in the file write them to scene description map
    // or object/material maps
    std::string line;
    int mtlmaterialIdx = -1;
    int textureIdx = -1;
    while (std::getline(sceneFile, line)) {
        // Split each line with ' '
        // taking first element as key, and remaining elements as values related to that key
        std::string key;
        std::vector<std::string> values;
        std::string value;
        std::istringstream iss(line);
    	for (std::string s; iss >> s; )
    		values.push_back(s);
        if (values.size() > 0) {
            key = values[0];
            values.erase(values.begin(), values.begin()+1);
        }

        // Special case: key is material color
        // Needs to be saved so that all objects that come after use this color
        if (key == "mtlcolor")
            materialDescriptions.insert(materialDescriptions.begin()+(++mtlmaterialIdx), values);
        // Special case: key is texture
        // Needs to be saved so that all objects that come after use this texture
        else if (key == "texture")
            textureDescriptions.insert(textureDescriptions.begin()+(++textureIdx), values);
        // Special case: key is object
        // Needs to be saved with reference to which material color to use
        else if (key == "sphere")
            sphereDescriptions.push_back({ { mtlmaterialIdx, textureIdx }, values });
        // Special case: key is light
        // Needs to be saved in the light vector array
        else if (key == "light" || key == "attlight")
            lightDescriptions.push_back(values);
        // Special case: triangle
        else if (key == "v")
            vertexDescriptions.push_back(values);
        else if (key == "vn")
            vertexNormalDescriptions.push_back(values);
        else if (key == "vt")
            texCoordDescriptions.push_back(values);
        else if (key == "f")
            triangleDescriptions.push_back({ { mtlmaterialIdx, textureIdx }, values });
        // Otherwise add values as new entry in our scene description map
        else if (!key.empty())
            sceneDescription[key] = values;
    }
    sceneFile.close();

    // Attempt to create the scene from the parsed scene description,
    // ensuring all necessary info is provided and there are no errors

    // ----- Camera -----
    // eye
    Vector3 eyePosition;
    if (sceneDescription.find("eye") != sceneDescription.end() && sceneDescription["eye"].size() >= 3) {
        float position[3];
        for (int i = 0; i < 3; i++) {
            try {
                position[i] = std::stof(sceneDescription["eye"][i]);
            } catch (std::invalid_argument& e) {
                return EyeError;
            }
        }
        eyePosition = Vector3(position[0], position[1], position[2]);
    } else {
        return EyeError;
    }
    // viewdir
    Vector3 viewDirection;
    if (sceneDescription.find("viewdir") != sceneDescription.end() && sceneDescription["viewdir"].size() >= 3) {
        float direction[3];
        for (int i = 0; i < 3; i++) {
            try {
                direction[i] = std::stof(sceneDescription["viewdir"][i]);
            } catch (std::invalid_argument& e) {
                return ViewDirError;
            }
        }
        viewDirection = Vector3(direction[0], direction[1], direction[2]);
        if (viewDirection.Length() == 0) return ViewDirError;
    } else {
        return ViewDirError;
    }
    // updir
    Vector3 upDirection;
    if (sceneDescription.find("updir") != sceneDescription.end() && sceneDescription["updir"].size() >= 3) {
        float direction[3];
        for (int i = 0; i < 3; i++) {
            try {
                direction[i] = std::stof(sceneDescription["updir"][i]);
            } catch (std::invalid_argument& e) {
                return UpDirError;
            }
        }
        upDirection = Vector3(direction[0], direction[1], direction[2]);
        if (upDirection.Length() == 0) return UpDirError;
    } else {
        return UpDirError;
    }
    // vfov
    float fieldOfView;
    if (sceneDescription.find("vfov") != sceneDescription.end() && sceneDescription["vfov"].size() >= 1) {
        try {
            fieldOfView = std::stof(sceneDescription["vfov"][0]);
            if (fieldOfView <= 0) throw std::invalid_argument("vfov must be greater than 0.");
        } catch (std::invalid_argument& e) {
            return VFovError;
        }
    } else {
        return VFovError;
    }
    // imsize
    int width;
    int height;
    if (sceneDescription.find("imsize") != sceneDescription.end() && sceneDescription["imsize"].size() >= 2) {
        try {
            width = std::stoi(sceneDescription["imsize"][0]);
            height = std::stoi(sceneDescription["imsize"][1]);
            if (width <= 0 || height <= 0) throw std::invalid_argument("Height and width must be greater than 0.");
        } catch (std::invalid_argument& e) {
            return ImSizeError;
        }
    } else {
        return ImSizeError;
    }
    camera_ = Camera(eyePosition, viewDirection, upDirection, fieldOfView, width, height);

    // ----- Background Color -----
    if (sceneDescription.find("bkgcolor") != sceneDescription.end() && sceneDescription["bkgcolor"].size() >= 3) {
        float color[3];
        for (int i = 0; i < 3; i++) {
            try {
                color[i] = std::stof(sceneDescription["bkgcolor"][i]);
                if (color[i] < 0 || color[i] > 1.0) throw std::invalid_argument("Color rgb must be between 0->1.");
            } catch (std::invalid_argument& e) {
                return BkgColorError;
            }
        }
        backgroundColor_ = Color(color[0], color[1], color[2]);
    } else {
        return BkgColorError;
    }

    // ----- Depth Cueing -----
    if (sceneDescription.find("depthcueing") != sceneDescription.end() && sceneDescription["depthcueing"].size() >= 7) {
        float dc[3];
        float amax, amin;
        float distmax, distmin;
        try {
            for (int i = 0; i < 3; i++) {
                dc[i] = std::stof(sceneDescription["depthcueing"][i]);
                if (dc[i] < 0 || dc[i] > 1.0) throw std::invalid_argument("Color rgb must be between 0->1.");
            }
            amax = std::stof(sceneDescription["depthcueing"][3]);
            amin = std::stof(sceneDescription["depthcueing"][4]);
            if (amax < 0 || amax > 1.0) throw std::invalid_argument("amax invalid.");
            if (amin < 0 || amin > 1.0) throw std::invalid_argument("amin invalid.");
            distmax = std::stof(sceneDescription["depthcueing"][5]);
            distmin = std::stof(sceneDescription["depthcueing"][6]);
            if (distmax < distmin) throw std::invalid_argument("distmax less than distmin.");
        } catch (std::invalid_argument& e) {
            return DepthCueingError;
        }
        depthCueingColor_ = Color(dc[0], dc[1], dc[2]);
        aMax_ = amax;
        aMin_ = amin;
        distMax_ = distmax;
        distMin_ = distmin;
    }

    // ----- Scene Objects -----
    // Ensure all material colors are valid
    size_t materialCount = materialDescriptions.size();
    if (materialCount == 0) return MtlColorError;
    for (std::size_t c = 0; c < materialCount; c++) {
        if (materialDescriptions[c].size() >= 12) {
            float Od[3];
            float Os[3];
            float k[3];  // ka, kd, ks
            float n, a, ior;
            try {
                for (int i = 0; i < 3; i++) {
                    Od[i] = std::stof(materialDescriptions[c][i]);
                    Os[i] = std::stof(materialDescriptions[c][i+3]);
                    k[i] = std::stof(materialDescriptions[c][i+6]);
                    if (Od[i] < 0 || Od[i] > 1.0) throw std::invalid_argument("Color rgb must be between 0->1.");
                    if (Os[i] < 0 || Os[i] > 1.0) throw std::invalid_argument("Color rgb must be between 0->1.");
                    if (k[i] < 0 || k[i] > 1.0) throw std::invalid_argument("k must be between 0->1.");
                }
                n = std::stof(materialDescriptions[c][9]);
                a = std::stof(materialDescriptions[c][10]);
                ior = std::stof(materialDescriptions[c][11]);
            } catch (std::invalid_argument& e) {
                return MtlColorError;
            }
            Material material;
            material.Od = Color(Od[0], Od[1], Od[2]);
            material.Os = Color(Os[0], Os[1], Os[2]);
            material.ka = k[0];
            material.kd = k[1];
            material.ks = k[2];
            material.n = n;
            material.a = a;
            material.ior = ior;
            materials_.push_back(material);
        } else {
            return MtlColorError;
        }
    }
    // Load all textures
    size_t textureCount = textureDescriptions.size();
    for (std::size_t c = 0; c < textureCount; c++) {
        if (textureDescriptions[c].size() >= 1) {
            //try {
                textures_.push_back(Image::ReadPPM(textureDescriptions[c][0]));
            //} catch (...) {
            //    return TextureError;
            //}
        } else {
            return TextureError;
        }
    }

    // Attempt to process all spheres and assign colors
    for (size_t s = 0; s < sphereDescriptions.size(); s++) {
        int materialIdx = sphereDescriptions[s].first[0];
        int textureIdx = sphereDescriptions[s].first[1];
        std::vector<std::string> sphereDescription = sphereDescriptions[s].second;
        if (materialIdx >= 0 && sphereDescription.size() >= 4) {
            float position[3];
            float radius;
            try {
                for (int i = 0; i < 3; i++) {
                    position[i] = std::stof(sphereDescription[i]);
                }
                radius = std::stof(sphereDescription[3]);
            } catch (std::invalid_argument& e) {
                return SphereError;
            }
            Vector3 spherePosition = Vector3(position[0], position[1], position[2]);
            Sphere* sphere = new Sphere(spherePosition, radius, materialIdx, textureIdx);
            sceneObjects_.push_back(sphere);
        } else {
            return SphereError;
        }
    }

    // ----- Lights -----
    for (auto lightDescription : lightDescriptions) {
        if (lightDescription.size() >= 7) {
            float x, y, z;
            int w;
            float color[3];
            float c1 = -1, c2 = -1, c3 = -1;
            try {
                x = std::stof(lightDescription[0]);
                y = std::stof(lightDescription[1]);
                z = std::stof(lightDescription[2]);
                w = std::stoi(lightDescription[3]);
                if (w != 0 && w != 1) throw std::invalid_argument("w component must be either 0 or 1.");
                for (int i = 0; i < 3; i++) {
                    color[i] = std::stof(lightDescription[i+4]);
                    if (color[i] < 0) throw std::invalid_argument("Color rgb must be positive.");
                }
            } catch (std::invalid_argument& e) {
                return LightError;
            }
            try {
                if (lightDescription.size() >= 10) {
                    c1 = std::stof(lightDescription[7]);
                    c2 = std::stof(lightDescription[8]);
                    c3 = std::stof(lightDescription[9]);
                    if (c1 < 0 || c2 < 0 || c3 < 0) throw std::invalid_argument("c1, c2, c3 must be positive.");
                }
            } catch (std::invalid_argument& e) {}
            Color lightColor = Color(color[0], color[1], color[2]);
            if (w == 0) {  // Directional light
                Vector3 lightDirection = Vector3(x, y, z);
                DirectionalLight directionalLight = DirectionalLight(lightDirection, lightColor);
                directionalLights_.push_back(directionalLight);
            } else {  // Point light
                Vector3 lightPosition = Vector3(x, y, z);
                PointLight pointLight;
                if (c1 == -1 || c2 == -1 || c3 == -1)
                    pointLight = PointLight(lightPosition, lightColor);
                else
                    pointLight = PointLight(lightPosition, lightColor, c1, c2, c3);
                pointLights_.push_back(pointLight);
            }
        } else {
            return LightError;
        }
    }

    // ----- Vertices -----
    Vector3 vertices[vertexDescriptions.size()];
    for (std::size_t v = 0; v < vertexDescriptions.size(); v++) {
        if (vertexDescriptions[v].size() >= 3) {
            float x, y, z;
            try {
                x = std::stof(vertexDescriptions[v][0]);
                y = std::stof(vertexDescriptions[v][1]);
                z = std::stof(vertexDescriptions[v][2]);
            } catch (std::invalid_argument& e) {
                return VertexError;
            }
            vertices[v] = Vector3(x, y, z);
        } else {
            return VertexError;
        }
    }
    // ----- Normals -----
    Vector3 normals[vertexNormalDescriptions.size()];
    for (std::size_t n = 0; n < vertexNormalDescriptions.size(); n++) {
        if (vertexNormalDescriptions[n].size() >= 3) {
            float x, y, z;
            try {
                x = std::stof(vertexNormalDescriptions[n][0]);
                y = std::stof(vertexNormalDescriptions[n][1]);
                z = std::stof(vertexNormalDescriptions[n][2]);
            } catch (std::invalid_argument& e) {
                return NormalError;
            }
            normals[n] = Vector3(x, y, z);
        } else {
            return NormalError;
        }
    }
    // ----- Tex Coords -----
    Vector3 texCoords[texCoordDescriptions.size()];
    for (std::size_t c = 0; c < texCoordDescriptions.size(); c++) {
        if (texCoordDescriptions[c].size() >= 2) {
            float u, v;
            try {
                u = std::stof(texCoordDescriptions[c][0]);
                v = std::stof(texCoordDescriptions[c][1]);
            } catch (std::invalid_argument& e) {
                return TexCoordError;
            }
            texCoords[c] = Vector3(u, v, 0);
        } else {
            return TexCoordError;
        }
    }

    for (size_t t = 0; t < triangleDescriptions.size(); t++) {
        int materialIdx = triangleDescriptions[t].first[0];
        int textureIdx = triangleDescriptions[t].first[1];
        std::vector<std::string> triangleDescription = triangleDescriptions[t].second;
        if (materialIdx >= 0 && triangleDescription.size() >= 3) {
            Vector3 triangleVertices[3];
            bool hasNormals = false;
            Vector3 triangleNormals[3];
            bool hasTexCoords = false;
            Vector3 triangleTexCoords[3];
            try {
                for (int i = 0; i < 3; i++) {
                    std::vector<std::string> vertexInfo = Utilities::SplitString(triangleDescription[i], "/");
                    if (vertexInfo.size() == 0)
                        return TriangleError;
                    int vertexIdx = std::stoi(vertexInfo[0])-1;
                    triangleVertices[i] = vertices[vertexIdx];

                    if (vertexInfo.size() > 1 && !vertexInfo[1].empty()) {
                        int texCoordsIdx = std::stoi(vertexInfo[1])-1;
                        triangleTexCoords[i] = texCoords[texCoordsIdx];
                        hasTexCoords = true;
                    }

                    if (vertexInfo.size() > 2 && !vertexInfo[2].empty()) {
                        int normalIdx = std::stoi(vertexInfo[2])-1;
                        triangleNormals[i] = normals[normalIdx];
                        hasNormals = true;
                    }
                }
            } catch (std::invalid_argument& e) {
                return TriangleError;
            }
            Triangle* triangle = new Triangle(triangleVertices, triangleNormals, triangleTexCoords, materialIdx, textureIdx, hasNormals, hasTexCoords);
            sceneObjects_.push_back(triangle);
        } else {
            return TriangleError;
        }
    }

    // If we made it this far, the scene has been successfully loaded
    return Success;
}

void Scene::AddObjectToScene(SceneObject* sceneObject) {
    sceneObjects_.push_back(sceneObject);
}
void Scene::AddPointLightToScene(PointLight pointLight) {
    pointLights_.push_back(pointLight);
}
void Scene::AddDirectionalLightToScene(DirectionalLight directionalLight) {
    directionalLights_.push_back(directionalLight);
}

float Scene::InShadow(Vector3 point, Vector3 lightPosition, const SceneObject* ignoreObject) const {
    float S = 0;

    if (softShadows_) {
        for (int i = 0; i < SHADOW_SAMPLE_COUNT; i++) {
            float x = (float(rand())/float((RAND_MAX))) - 0.25;
            float y = (float(rand())/float((RAND_MAX))) - 0.25;
            float z = (float(rand())/float((RAND_MAX))) - 0.25;
            Vector3 lightOffsetPosition = lightPosition + Vector3(x,y,z);
            Ray shadowRay = Ray(point, lightOffsetPosition-point);
            RaycastHit shadowHit = Raycast(shadowRay, ignoreObject);
            S += shadowHit.hit ? 0 : 1.0f/SHADOW_SAMPLE_COUNT;
        }
    } else {
        Ray shadowRay = Ray(point, lightPosition-point);
        RaycastHit shadowHit = Raycast(shadowRay, ignoreObject);
        S += shadowHit.hit ? (1-materials_[shadowHit.materialIdx].a) : 1;
    }

    if (S > 1) S = 1;
    return S;
}

Vector3 Scene::ComputeDiffuseSpecular(Vector3 L, Vector3 N, Vector3 I, Vector3 Od, Vector3 Os, float ka, float kd, float ks, float n) const {
    Vector3 H = Vector3::Normalize(L + I);
    Vector3 diffuse = kd*Od * std::max(0.0f, Vector3::Dot(N, L));
    Vector3 specular = ks*Os * std::pow(std::max(0.0f, Vector3::Dot(N, H)), n);
    return diffuse + specular;
}

Color Scene::TraceRay(const Ray ray, int iteration, const SceneObject* ignoreObject) const {
    // Raycast into the scene and get hit information
    RaycastHit raycastHit = Raycast(ray, ignoreObject);

    // Return the background color if no object was hit
    if (!raycastHit.hit)
        return backgroundColor_;

    // Convert hit object material parameters to vec3s
    Material hitMaterial = materials_[raycastHit.materialIdx];
    Vector3 Od;
    if (raycastHit.textureIdx != -1) {
        Color textureColor = textures_[raycastHit.textureIdx]->GetPixel(raycastHit.u, raycastHit.v);
        Od = Vector3(textureColor.r(), textureColor.g(), textureColor.b());
    } else {
        Od = Vector3(hitMaterial.Od.r(), hitMaterial.Od.g(), hitMaterial.Od.b());
    }
    Vector3 Os = Vector3(hitMaterial.Os.r(), hitMaterial.Os.g(), hitMaterial.Os.b());
    float ka = hitMaterial.ka;
    float kd = hitMaterial.kd;
    float ks = hitMaterial.ks;
    float n = hitMaterial.n;
    float a = hitMaterial.a;
    float ior = hitMaterial.ior;
    Vector3 I = -ray.Direction();
    Vector3 N = raycastHit.normal;

    // Determine if ray is leaving or entering based on normal
    bool leaving = Vector3::Dot(I, raycastHit.normal) > 0 ? false : true;
    if (leaving) N = -N;

    // Ambient light contribution
    Vector3 rayColor = ka*Od;
    // Point light contribution
    for (auto pointLight : pointLights_) {
        Vector3 IL = Vector3(pointLight.LightColor().r(), pointLight.LightColor().g(), pointLight.LightColor().b());
        Vector3 L = Vector3::Normalize(pointLight.Position()-raycastHit.point);
        Vector3 ds = ComputeDiffuseSpecular(L, N, I, Od, Os, ka, kd, ks, n);
        float S = InShadow(raycastHit.point, pointLight.Position(), raycastHit.object);
        float f = pointLight.Attenuate(raycastHit.point);
        rayColor = rayColor + S*f*IL*ds;
    }
    // Directional light contribution
    for (auto directionalLight : directionalLights_) {
        Vector3 IL = Vector3(directionalLight.LightColor().r(), directionalLight.LightColor().g(), directionalLight.LightColor().b());
        Vector3 L = -directionalLight.Direction();
        Vector3 ds = ComputeDiffuseSpecular(L, N, I, Od, Os, ka, kd, ks, n);
        float S = InShadow(raycastHit.point, raycastHit.point+(25*L), raycastHit.object);
        rayColor = rayColor + S*IL*ds;
    }
    // Reflectance contribution
    if (iteration < MAX_DEPTH) {
        float cosThetai = Vector3::Dot(N, I);
        Vector3 R = 2*cosThetai*N-I;
        Color Rc = TraceRay(Ray(raycastHit.point, R), ++iteration, raycastHit.object);
        float F0 = ((ior-1)/(ior+1))*((ior-1)/(ior+1));
        float Fr = F0 + (1-F0)*std::pow((1-cosThetai), 5);
        rayColor = rayColor + Fr*Vector3(Rc.r(), Rc.g(), Rc.b());

        float ni = leaving ? IOR_AIR : ior;
        float nt = leaving ? ior : IOR_AIR;
        float tir = 1-((ni/nt)*(ni/nt)*(1-cosThetai*cosThetai));
        if (tir >= 0)
        {
            float cosThetat = std::sqrt(tir);
            Vector3 T = cosThetat*(-N) + (ni/nt)*(cosThetai*N-I);
            Color Tc = TraceRay(Ray(raycastHit.point+T*0.0001, T), ++iteration);
            rayColor = rayColor + (1 - Fr)*(1 - a)*Vector3(Tc.r(), Tc.g(), Tc.b());
        }
    }

    // Return final color after depth cueing
    return Color(rayColor.x(), rayColor.y(), rayColor.z()); //DepthCue(I, raycastHit.distance);
}

Image Scene::Render() {
    // Pull variables from scene
    int pixelWidth = camera_.Width();
    int pixelHeight = camera_.Height();
    Vector3 eyePosition = camera_.EyePosition();
    Vector3 viewDirection = camera_.ViewDirection();
    Vector3 upDirection = camera_.UpDirection();

    // Initialize the final output render image
    Image renderImage = Image(pixelWidth, pixelHeight);

    // Calculate height and width of viewing window in world space
    float vfovRadians = camera_.FieldOfView() * (M_PI/180);
    float h = 2*viewingDistance_*tan(vfovRadians/2);
    float w = h * (pixelWidth / (float)pixelHeight);
    // Compute u and v directions of viewing window
    Vector3 u = Vector3::Normalize(Vector3::Cross(viewDirection, upDirection));
    Vector3 v = Vector3::Normalize(Vector3::Cross(u, viewDirection));

    // Compute viewing window corner positions in world space
    Vector3 ul = eyePosition + viewingDistance_*viewDirection - (w/2)*u + (h/2)*v;
    Vector3 ur = eyePosition + viewingDistance_*viewDirection + (w/2)*u + (h/2)*v;
    Vector3 ll = eyePosition + viewingDistance_*viewDirection - (w/2)*u - (h/2)*v;
    //Vector3 lr = eyePosition + viewingDistance_*viewDirection + (w/2)*u - (h/2)*v;

    // Compute pixel offsets
    Vector3 du = u * Vector3::Distance(ur, ul)/pixelWidth;
    Vector3 dv = v * Vector3::Distance(ll, ul)/pixelHeight;

    // Iterate through each pixel in the output image and trace a ray to determine pixel color
    for (int x = 0; x < renderImage.Width(); x++) {
        for (int y = 0; y < renderImage.Height(); y++) {
            // Calculate position of viewing window pixel in world space
            Vector3 pixelPosition = ul + x*du - y*dv + du/2 - dv/2;
            Color avgColor;
            float dofJitter = depthOfField_ ? 0.075f : 0;
            int dofIterations = depthOfField_ ? DOF_SAMPLE_COUNT : 1;
            for (int i = 0; i < dofIterations; i++)
            {
                float x = (float(rand())/float((RAND_MAX))) * dofJitter - dofJitter/2;
                float y = (float(rand())/float((RAND_MAX))) * dofJitter - dofJitter/2;
                float z = (float(rand())/float((RAND_MAX))) * dofJitter - dofJitter/2;
                Vector3 eyeOffset = eyePosition + Vector3(x, y, z);
                // Calculate ray from eye through pixel
                Ray viewingRay = Ray(eyeOffset, pixelPosition - eyeOffset);
                // Trace the ray to set the pixel color
                avgColor = avgColor + TraceRay(viewingRay)/dofIterations;
            }
            avgColor.Clamp01();
            renderImage.SetPixel(x, y, avgColor);
        }
    }

    // Return the rendered image
    return renderImage;
}

RaycastHit Scene::Raycast(const Ray ray, const SceneObject* ignoreObject) const {


    RaycastHit closestHitInfo = RaycastBVH(ray, bvhRoot_, ignoreObject);

    // Return the raycast hit information
    return closestHitInfo;
}

RaycastHit Scene::RaycastBVH(const Ray ray, BVHNode* node, const SceneObject* ignoreObject) const {
    // If the ray intersects with the BVH
    // Leaf - check for intersection with object
    // Non-leaf - recurse further
    if (node->IntersectsRay(ray)) {
        if (node->IsLeaf()) {
            if (node->IsEmpty() || node->Leaf() == ignoreObject)
                return RaycastHit();
            else
                return node->Leaf()->IntersectRay(ray);
        } else {
            RaycastHit left = RaycastBVH(ray, node->Left(), ignoreObject);
            RaycastHit right = RaycastBVH(ray, node->Right(), ignoreObject);
            return left.distance < right.distance ? left : right;
        }
    } else {
        return RaycastHit();
    }
}

Color Scene::DepthCue(Vector3 I, float d) const {
    float a;
    if (d <= distMin_) a = aMax_;
    else if (d >= distMax_) a = aMin_;
    else a = aMin_ + (aMax_-aMin_)*(distMax_-d)/(distMax_-distMin_);
    I = a*I + (1-a)*Vector3(depthCueingColor_.r(), depthCueingColor_.g(), depthCueingColor_.b());
    return Color(I.x(), I.y(), I.z());
}

void Scene::ConstructBVH() {
    bvhRoot_ = ConstructBVHRecursive(sceneObjects_);
    // for (size_t i = 0; i < sceneObjects_.size(); i++) {
    //     AABB objectBounds = sceneObjects_[i]->BoundingBox();
    //     std::cout << " ----- \n" << i << std::endl;
    //     std::cout << "MIN: < " << objectBounds.Min().x() << ", " << objectBounds.Min().y() << ", " << objectBounds.Min().z() << " > \n";
    //     std::cout << "MAX: < " << objectBounds.Max().x() << ", " << objectBounds.Max().y() << ", " << objectBounds.Max().z() << " > \n";
    // }
}

BVHNode* Scene::ConstructBVHRecursive(std::vector<SceneObject*>& sceneObjects) {
    // Special Case: Empty node
    if (sceneObjects.size() == 0)
        return new BVHNode();

    // Special Case: Leaf node
    if (sceneObjects.size() == 1)
        return new BVHNode(sceneObjects[0]->BoundingBox(), sceneObjects[0]);

    // Construct an AABB surrounding all the given objects
    AABB objectBounds = sceneObjects[0]->BoundingBox();
    Vector3 min = objectBounds.Min();
    Vector3 max = objectBounds.Max();
    for (size_t i = 1; i < sceneObjects.size(); i++) {
        objectBounds = sceneObjects[i]->BoundingBox();
        min = Vector3::Min(min, objectBounds.Min());
        max = Vector3::Max(max, objectBounds.Max());
    }
    AABB aabb = AABB(min, max);

    // Split the AABB in half along its longest side
    std::vector<SceneObject*> leftObjects;
    std::vector<SceneObject*> rightObjects;
    Vector3 dims = max-min;
    float longestAxis = std::max(dims.x(), std::max(dims.y(), dims.z()));
    if (dims.x() == longestAxis)
        std::sort(sceneObjects.begin(), sceneObjects.end(), CompareObectX);
    else if (dims.y() == longestAxis)
        std::sort(sceneObjects.begin(), sceneObjects.end(), CompareObectY);
    else
        std::sort(sceneObjects.begin(), sceneObjects.end(), CompareObectZ);
    for (size_t i = 0; i < sceneObjects.size(); i++) {
        if (i < sceneObjects.size()/2)
            leftObjects.push_back(sceneObjects[i]);
        else
            rightObjects.push_back(sceneObjects[i]);
    }

    return new BVHNode(aabb, ConstructBVHRecursive(leftObjects), ConstructBVHRecursive(rightObjects));
}

}  // namespace RayTracer
