//
// Created by kskun on 2021/11/28.
//

#ifndef ASSIGNMENT4_RAY_TRACER_H
#define ASSIGNMENT4_RAY_TRACER_H

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "global.h"
#include "grid.h"

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight) :
            scene(s), group(s->getGroup()), maxBounces(max_bounces), cutoffWeight(cutoff_weight) {
        assert(gridNX != -1 && gridNY != -1 && gridNZ != -1 ||
               gridNX == -1 && gridNY == -1 && gridNZ == -1);
        if (gridNX != -1) { // grid set
            grid = new Grid(group->getBoundingBox(), gridNX, gridNY, gridNZ);
            group->insertIntoGrid(grid, nullptr);
        }
    }

    ~RayTracer() {
        delete grid;
    }

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;

    Grid *getGrid() { return grid; }

protected:
    SceneParser *scene;
    Group *group;
    int maxBounces;
    float cutoffWeight;
    Grid *grid;
};


#endif //ASSIGNMENT4_RAY_TRACER_H
