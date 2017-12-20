#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "OSExport.h"
#include "LinearMath.h"
#include <vector>

class ENGINE_API Transform {

public:
    Transform();
    ~Transform();

    void setLocalPosition(Vector3 localPosition);
    void setLocalRotation(Quaternion localRotation);
    void setLocalScale(Vector3 localScale);
    void setParent(Transform* parent);
    void addChild(Transform* child);
    void removeChild(Transform* child);
    
    
    const Vector3& getLocalPosition() const;
    const Quaternion& getLocalRotation() const;
    const Vector3& getLocalScale() const;
    const Matrix4& getWorldMatrix() const;
    const Transform* const getParent() const;
    Transform* getChildren();
    unsigned int getChildrenCount();
    
    Vector3 getFront() const;
    Vector3 getUp() const;
    Vector3 getRight() const;

private:
    /* DON'T DEALLOCATE ON EXIT, IT'S NOT OWNED */
    Transform* mParent = nullptr;
    std::vector<Transform*> mChildren;
    Vector3 mLocalPosition;
    Quaternion mLocalRotation;
    Vector3 mLocalScale;
    mutable Matrix4 mWorldMatrix;
    mutable bool mValidCalcCachedMatrix;
};

#endif /*TRANSFORM_H*/