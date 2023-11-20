#include "bvh_node.h"

namespace RayTracer {

BVHNode::BVHNode() {
    isLeaf_ = true;
    isEmpty_ = true;
}

BVHNode::BVHNode(AABB aabb, BVHNode* left, BVHNode* right) {
    isLeaf_ = false;
    isEmpty_ = false;
    aabb_ = aabb;
    left_ = left;
    right_ = right;
}

BVHNode::BVHNode(AABB aabb, SceneObject* leaf) {
    isLeaf_ = true;
    isEmpty_ = false;
    aabb_ = aabb;
    leaf_ = leaf;
}

BVHNode::~BVHNode() {
    if (!isLeaf_) {
        delete left_;
        delete right_;
    }
}


}  // namespace RayTracer
