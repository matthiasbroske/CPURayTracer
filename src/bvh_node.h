#ifndef BVH_NODE_H_
#define BVH_NODE_H_

#include "aabb.h"
#include "scene_object.h"

namespace RayTracer {

/// Bounding volume hierachy node.
class BVHNode {
public:
    BVHNode();
    BVHNode(AABB aabb, BVHNode* left, BVHNode* right);
    BVHNode(AABB aabb, SceneObject* leaf);
    ~BVHNode();

    BVHNode* Left() const { return left_; }
    BVHNode* Right() const { return right_; }
    bool IsLeaf() const { return isLeaf_; }
    bool IsEmpty() const { return isEmpty_; }
    SceneObject* Leaf() const { return leaf_; }

    bool IntersectsRay(Ray ray) const { return aabb_.IntersectsRay(ray); }

private:
    AABB aabb_;
    BVHNode* left_;
    BVHNode* right_;
    bool isEmpty_;
    bool isLeaf_;
    SceneObject* leaf_;
};

}  // namespace RayTracer

#endif  // BVH_NODE_H_
