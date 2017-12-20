#include "Transform.h"
#include <iostream>

Transform::Transform() {
    mValidCalcCachedMatrix = false;
    mWorldMatrix = Matrix4(1);
    mLocalPosition = Vector3(0, 0, 0);
    mLocalScale = Vector3(1, 1, 1);
    mLocalRotation = Quaternion(0, 0, 0, 0);
}

Transform::~Transform() {

}

void Transform::setLocalPosition(Vector3 localPosition) {
    mValidCalcCachedMatrix = false;
    mLocalPosition = localPosition;
}

void Transform::setLocalRotation(Quaternion localRotation) {
    mValidCalcCachedMatrix = false;
    mLocalRotation = localRotation;
}

void Transform::setLocalScale(Vector3 localScale) {
    mValidCalcCachedMatrix = false;
    mLocalScale = localScale;
}

void Transform::setParent(Transform* parent) {
    // TODO: MAYBE RECALCULATE TRANSFORM OF CHILD
    mValidCalcCachedMatrix = false;
    mParent = parent;
    if (mParent != nullptr)
        mParent->addChild(this);
}

void Transform::addChild(Transform* child) {
    if (child != nullptr) mChildren.push_back(child);
}

void Transform::removeChild(Transform* child) {
    unsigned int childToRemoveIndex = -1;
    while (childToRemoveIndex + 1 < mChildren.size()) {
        childToRemoveIndex++;
        if (mChildren[childToRemoveIndex] == child) break;
    }

    if (childToRemoveIndex >= 0) {
        mChildren.erase(mChildren.begin() + childToRemoveIndex);
    }
}

const Vector3& Transform::getLocalPosition() const {
    return mLocalPosition;
}

const Quaternion& Transform::getLocalRotation() const {
    return mLocalRotation;
}

const Vector3& Transform::getLocalScale() const {
    return mLocalScale;
}

const Matrix4& Transform::getWorldMatrix() const {
    if (!mValidCalcCachedMatrix) {
        // TODO: TAKE ACCOUNT OF PARENTAL FOR LOCAL CALCULATIONS
        // TODO: CALCULATE MODEL MATRIX USING POSITION, ROTATION AND SCALE
        mWorldMatrix = Matrix4(1);
        mWorldMatrix = Math::translate(mWorldMatrix, mLocalPosition);
        Vector3 euler = Math::eulerAngles(mLocalRotation);
        mWorldMatrix = Math::rotate(mWorldMatrix, euler.x, Vector3(1, 0, 0));
        mWorldMatrix = Math::rotate(mWorldMatrix, euler.y, Vector3(0, 1, 0));
        mWorldMatrix = Math::rotate(mWorldMatrix, euler.z, Vector3(0, 0, 1));
        //mWorldMatrix = Math::rotate(mWorldMatrix, mLocalRotation.w, direction);    
        mWorldMatrix = Math::scale(mWorldMatrix, mLocalScale);
        mValidCalcCachedMatrix = true;
    }
    return mWorldMatrix;
}

const Transform* const Transform::getParent() const {
    return mParent;
}

Transform* Transform::getChildren() {
    return mChildren[0];
}

unsigned int Transform::getChildrenCount() {
    return (unsigned int)mChildren.size();
}

Vector3 Transform::getFront() const {
    return Vector3(mWorldMatrix[0][2], mWorldMatrix[1][2], mWorldMatrix[2][2]);
}

Vector3 Transform::getUp() const {
    return  Vector3(mWorldMatrix[0][1], mWorldMatrix[1][1], mWorldMatrix[2][1]);
}

Vector3 Transform::getRight() const {
    return -Vector3(mWorldMatrix[0][0], mWorldMatrix[1][0], mWorldMatrix[2][0]);
}