//
// Created by kskun on 2021/11/28.
//

#ifndef ASSIGNMENT4_RAY_TRACER_H
#define ASSIGNMENT4_RAY_TRACER_H

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "group.h"

class RayTracer {
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight) :
            scene(s), group(s->getGroup()), maxBounces(max_bounces), cutoffWeight(cutoff_weight) {}

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const;

protected:
    SceneParser *scene;
    Group *group;
    int maxBounces;
    float cutoffWeight;
};


#endif //ASSIGNMENT4_RAY_TRACER_H
