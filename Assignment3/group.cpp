//
// Created by kskun on 2021/10/4.
//

#include "group.h"
#include "global.h"

void Group::addObject(int index, Object3D *object) {
    assert(index < objectNum);
    objects[index] = object;
}

bool Group::intersect(const Ray &r, Hit &h, float tMin) {
    Hit hTmp;
    float tTmp = FP_INFINITE;
    bool flagHasInter = false;
    for (int i = 0; i < objectNum; i++) {
        auto obj = objects[i];
        if (obj->intersect(r, hTmp, tMin) && hTmp.getT() < tTmp + EPSILON) {
            flagHasInter = true;
            h = hTmp;
            tTmp = h.getT();
        }
    }
    return flagHasInter;
}
