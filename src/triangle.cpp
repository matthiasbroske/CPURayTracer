#include "triangle.h"
    #include <iostream>

namespace RayTracer {

Triangle::Triangle(Vector3 vertices[3], Vector3 normals[3], Vector3 texCoords[3], int materialIdx, int textureIdx, bool hasNormals, bool hasTexCoords) : SceneObject((vertices[0]+vertices[1]+vertices[2])/3, materialIdx, textureIdx){
    hasNormals_ = hasNormals;
    hasTexCoords_ = hasTexCoords;

    vertices_[0] = vertices[0];
    vertices_[1] = vertices[1];
    vertices_[2] = vertices[2];

    if (hasNormals) {
        normals_[0] = normals[0];
        normals_[1] = normals[1];
        normals_[2] = normals[2];
    }
    normal_ = Vector3::Cross(vertices[1]-vertices[0], vertices[2]-vertices[0]);
    normal_.Normalize();
    d_ = Vector3::Dot(normal_, -vertices_[0]);

    if (hasTexCoords) {
        texCoords_[0] = texCoords[0];
        texCoords_[1] = texCoords[1];
        texCoords_[2] = texCoords[2];
    }
}

AABB Triangle::BoundingBox() const {
    return AABB(vertices_[0], vertices_[1], vertices_[2]);
}

RaycastHit Triangle::IntersectRay(Ray ray) const {
    // Initialize raycast hit info
    RaycastHit hitInfo;
    hitInfo.hit = false;
    hitInfo.distance = std::numeric_limits<float>::infinity();

    // Does ray intersect the plane this triangle lies within
    float t = -(Vector3::Dot(normal_, ray.Origin()) + d_) / Vector3::Dot(normal_, ray.Direction());
    if (t < 0)
        return hitInfo;

    // Does the ray intersection point lie within the triangle
    Vector3 p = ray.GetPoint(t);
    Vector3 e1 = vertices_[1]-vertices_[0];
    Vector3 e2 = vertices_[2]-vertices_[0];
    Vector3 ep = p-vertices_[0];
    float d11 = Vector3::Dot(e1, e1);
    float d12 = Vector3::Dot(e1, e2);
    float d22 = Vector3::Dot(e2, e2);
    float dp1 = Vector3::Dot(ep, e1);
    float dp2 = Vector3::Dot(ep, e2);
    float D = d11*d22-d12*d12;
    float Db = d22*dp1-d12*dp2;
    float Dy = d11*dp2-d12*dp1;

    float b = Db/D;
    float y = Dy/D;
    float a = 1-(b+y);

    if (a < 0 || a > 1 ||b < 0 || b > 1 || y < 0 || y > 1)
        return hitInfo;

    hitInfo.hit = true;
    hitInfo.distance = Vector3::Distance(ray.Origin(), p);
    hitInfo.point = p;
    hitInfo.normal = hasNormals_ ? a*normals_[0] + b*normals_[1] + y*normals_[2] : normal_;
    hitInfo.materialIdx = materialIdx_;
    if (hasTexCoords_) {
        Vector3 interpolatedCoords = a*texCoords_[0] + b*texCoords_[1] + y*texCoords_[2];
        hitInfo.u = interpolatedCoords.x();
        hitInfo.v = interpolatedCoords.y();
        hitInfo.textureIdx = textureIdx_;
    } else {
        hitInfo.textureIdx = -1;
    }
    hitInfo.object = this;

    return hitInfo;
}

}  // namespace RayTracer
