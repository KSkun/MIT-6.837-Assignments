//
// Created by kskun on 2021/10/11.
//

#include "transform.h"

void Transform::TransformPosition(Vec3f &dst, const Vec3f &src, const Matrix &mat) {
    auto src4 = Vec4f(src.x(), src.y(), src.z(), 1);
    mat.Transform(src4);
    dst = Vec3f(src4.x() / src4.w(), src4.y() / src4.w(), src4.z() / src4.w());
}

void Transform::TransformDirection(Vec3f &dst, const Vec3f &src, const Matrix &mat) {
    auto src4 = Vec4f(src.x(), src.y(), src.z(), 0);
    mat.Transform(src4);
    dst = Vec3f(src4.x(), src4.y(), src4.z());
}

bool Transform::intersect(const Ray &r, Hit &h, float tMin) {
    Vec3f nRo, nRd;
    TransformPosition(nRo, r.getOrigin(), matInv);
    TransformDirection(nRd, r.getDirection(), matInv);
    auto transformedRdLen = nRd.Length();
    nRd.Normalize();
    auto nRay = Ray(nRo, nRd);

    if (!obj->intersect(nRay, h, tMin)) return false;
    auto t = h.getT() / transformedRdLen;

    Vec3f n = h.getNormal();
    TransformDirection(n, h.getNormal(), matInvT);
    n.Normalize();
    h.set(t, h.getMaterial(), n, r);
    return true;
}