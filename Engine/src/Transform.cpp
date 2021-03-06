#include "Transform.h"
#include <iostream>

Transform::Transform() {
    mValidCalcCachedMatrix = false;
    mWorldMatrix = Matrix4(1);
    mLocalPosition = Vector3(0, 0, 0);
    mLocalScale = Vector3(1, 1, 1);
    mLocalRotation = Quaternion(0, 0, 0, 0);
	mMaxChildren = 64;
	mChildCount = 0;

	mChildren = new Transform*[mMaxChildren];
}

Transform::Transform(const Transform& other)
{
	memcpy(this, &other, sizeof(Transform));
}

Transform::~Transform() 
{

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
	if (child != nullptr) {
		if (mChildCount >= mMaxChildren) {
			mMaxChildren *= 2;
			Transform** tempChildrenContainer;
			tempChildrenContainer = new Transform*[mMaxChildren];

			memcpy(tempChildrenContainer, mChildren, mMaxChildren);
			
			delete mChildren;

			mChildren = tempChildrenContainer;
		}

		uint childNumber = mChildCount;
		mChildren[childNumber] = child;
		mChildCount++;
	}
}

void Transform::removeChild(Transform* child) {
    uint childToRemoveIndex = -1;
    while (childToRemoveIndex + 1 < mChildCount) {
        childToRemoveIndex++;
        if (mChildren[childToRemoveIndex] == child) break;
    }

    if (childToRemoveIndex >= 0) {

		// Copy next to current location
		for (uint i = childToRemoveIndex; i < mChildCount - 1; i++)
			mChildren[i] = mChildren[i + 1];
    }
}

void Transform::lookAt(Vector3 position)
{
	Vector3 orientation = Math::normalize(position - mLocalPosition);
	setLocalRotation(orientation);
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

const Matrix4& Transform::getWorldInverseTranspose() const
{
	if (!mValidCalcCachedMatrix)
	{
		const Vector3& t = mLocalPosition;
		const Vector3& s = mLocalScale;
		glm::mat3 r = Math::toMat3(mLocalRotation);
		mWorldInverseTransposeMatrix = 
		{
			{r[0][0] / s.x, r[0][1] / s.x, r[0][2] / s.x, 0},
			{r[1][0] / s.y, r[1][1] / s.y, r[1][2] / s.y, 0},
			{r[2][0] / s.x, r[2][1] / s.x, r[2][2] / s.x, 0},
			{-t.x, -t.y, -t.z, 1}
		};
	}

	return mWorldInverseTransposeMatrix;
}

const Transform* const Transform::getParent() const {
    return mParent;
}

Transform* Transform::getChildren() const {
    return mChildren[0];
}

uint Transform::getChildrenCount() const {
	return mChildCount;
}

Vector3 Transform::getFront() const {
	return Math::eulerAngles(mLocalRotation);
}

Vector3 Transform::getUp() const {
    return  Vector3(mWorldMatrix[0][1], mWorldMatrix[1][1], mWorldMatrix[2][1]);
}

Vector3 Transform::getRight() const {
    return -Vector3(mWorldMatrix[0][0], mWorldMatrix[1][0], mWorldMatrix[2][0]);
}

bool Transform::isDirty() const
{
	return mValidCalcCachedMatrix;
}
