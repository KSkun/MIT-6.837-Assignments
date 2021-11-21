//
// Created by kskun on 2021/10/4.
//

#ifndef ASSIGNMENT1_GROUP_H
#define ASSIGNMENT1_GROUP_H

#include "object3d.h"

class Group : public Object3D {
public:
    explicit Group(int objectNum) : objectNum(objectNum) {
        assert(objectNum > 0);
        objects = new Object3D *[objectNum];
    }

    Group() : Group(1) {}

    ~Group() {
        delete[] objects;
    }

    void addObject(int index, Object3D *object);

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

private:
    int objectNum;
    Object3D **objects;
};

#endif //ASSIGNMENT1_GROUP_H
