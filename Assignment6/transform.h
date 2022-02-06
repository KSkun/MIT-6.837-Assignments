//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_TRANSFORM_H
#define ASSIGNMENT2_TRANSFORM_H

#include <algorithm>

#include "object3d.h"
#include "matrix.h"
#include "global.h"

class Transform : public Object3D {
public:
    Transform(Matrix &m, Object3D *o) : Object3D(), mat(m), obj(o) {
        mat.Inverse(matInv, EPSILON);
        matInv.Transpose(matInvT);

        auto objBBox = obj->getBoundingBox();
        auto objMax = objBBox->getMax(), objMin = objBBox->getMin();
        auto diffX = objMax.x() - objMin.x(),
                diffY = objMax.y() - objMin.y();
        auto p1 = objBBox->getMin(),
                p2 = p1 + Vec3f(diffX, 0, 0),
                p3 = p1 + Vec3f(0, diffY, 0),
                p4 = p1 + Vec3f(diffX, diffY, 0);
        auto p5 = objBBox->getMax(),
                p6 = p5 - Vec3f(diffX, 0, 0),
                p7 = p5 - Vec3f(0, diffY, 0),
                p8 = p5 - Vec3f(diffX, diffY, 0);
        Vec3f p1t, p2t, p3t, p4t, p5t, p6t, p7t, p8t;
        TransformPosition(p1t, p1, mat);
        TransformPosition(p2t, p2, mat);
        TransformPosition(p3t, p3, mat);
        TransformPosition(p4t, p4, mat);
        TransformPosition(p5t, p5, mat);
        TransformPosition(p6t, p6, mat);
        TransformPosition(p7t, p7, mat);
        TransformPosition(p8t, p8, mat);
        auto minmaxX = std::minmax({p1t.x(), p2t.x(), p3t.x(), p4t.x(),
                                    p5t.x(), p6t.x(), p7t.x(), p8t.x()}),
                minmaxY = std::minmax({p1t.y(), p2t.y(), p3t.y(), p4t.y(),
                                       p5t.y(), p6t.y(), p7t.y(), p8t.y()}),
                minmaxZ = std::minmax({p1t.z(), p2t.z(), p3t.z(), p4t.z(),
                                       p5t.z(), p6t.z(), p7t.z(), p8t.z()});
        bbox = new BoundingBox(Vec3f(minmaxX.first, minmaxY.first, minmaxZ.first),
                               Vec3f(minmaxX.second, minmaxY.second, minmaxZ.second));
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    void insertIntoGrid(Grid *g, Matrix *m) override;

protected:
    static void TransformPosition(Vec3f &dst, const Vec3f &src, const Matrix &mat);

    static void TransformDirection(Vec3f &dst, const Vec3f &src, const Matrix &mat);

    Matrix mat, matInv, matInvT;
    Object3D *obj;
};

#endif //ASSIGNMENT2_TRANSFORM_H
