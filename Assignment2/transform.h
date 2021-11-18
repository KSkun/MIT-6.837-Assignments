//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_TRANSFORM_H
#define ASSIGNMENT2_TRANSFORM_H

#include "object3d.h"
#include "matrix.h"
#include "global.h"

class Transform : public Object3D {
public:
    Transform(Matrix &m, Object3D *o) : Object3D(), mat(m), obj(o) {
        mat.Inverse(matInv, EPSILON);
        matInv.Transpose(matInvT);
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

protected:
    static void TransformPosition(Vec3f &dst, const Vec3f &src, const Matrix &mat);

    static void TransformDirection(Vec3f &dst, const Vec3f &src, const Matrix &mat);

    Matrix mat, matInv, matInvT;
    Object3D *obj;
};

#endif //ASSIGNMENT2_TRANSFORM_H
