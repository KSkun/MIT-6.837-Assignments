//
// Created by kskun on 2021/11/28.
//

#include "ray_tracer.h"
#include "light.h"
#include "global.h"
#include "rayTree.h"
#include "raytracing_stats.h"

Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) {
    Vec3f result = incoming - 2 * incoming.Dot3(normal) * normal;
    result.Normalize();
    return result;
}

bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t,
                          Vec3f &transmitted) {
    auto revI = incoming;
    revI.Negate();
    float index_r = index_i / index_t;
    float nDotI = normal.Dot3(revI);
    float inSqrt = 1 - index_r * index_r * (1 - nDotI * nDotI);
    if (inSqrt < 0) return false;
    float coN = index_r * nDotI - sqrt(inSqrt), coI = -index_r;
    transmitted = coN * normal + coI * revI;
    transmitted.Normalize();
    return true;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
    RayTracingStats::IncrementNumNonShadowRays();

    // stop conditions
    // no intersection
    if (!group->intersect(ray, hit, tmin)) {
        return scene->getBackgroundColor();
    }
    // too many bounces
    if (bounces > maxBounces) {
        return {0, 0, 0};
    }
    // low weight
    if (fcmp(weight - cutoffWeight) < 0) {
        return {0, 0, 0};
    }

    if (weight == 1) { // main segment
        RayTree::SetMainSegment(ray, tmin, hit.getT());
    }

    // ambient
    Vec3f color = scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor(hit.getIntersectionPoint());

    // local shading
    for (int iLight = 0; iLight < scene->getNumLights(); iLight++) {
        // prepare light information
        auto light = scene->getLight(iLight);
        Vec3f dirToLight, lightColor;
        float disToLight;
        light->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, disToLight);

        // cast shadow ray and test if blocked
        Ray lightRay = {hit.getIntersectionPoint() - ray.getDirection() * 0.001f, dirToLight};
        Hit lightHit;
        RayTracingStats::IncrementNumShadowRays();
        if (shadows && group->intersect(lightRay, lightHit, 0) && fcmp(lightHit.getT() - disToLight) < 0) {
            RayTree::AddShadowSegment(lightRay, 0, lightHit.getT());
            continue;
        }
        if (shadows) RayTree::AddShadowSegment(lightRay, 0, disToLight);

        color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
    }

    auto material = dynamic_cast<PhongMaterial *>(hit.getMaterial());
    if (material != nullptr) {
        // reflections
        auto reflectiveColor = material->getReflectiveColor(hit.getIntersectionPoint());
        if (reflectiveColor.Length() > 0) { // reflective material
            Ray reflectiveRay = {hit.getIntersectionPoint() - ray.getDirection() * 0.001f,
                                 mirrorDirection(hit.getNormal(), ray.getDirection())};
            Hit reflectiveHit;
            auto reflectiveResult = traceRay(reflectiveRay, 0, bounces + 1,
                                             weight * reflectiveColor.Length(), indexOfRefraction, reflectiveHit);
            float reflectiveT = 1e10;
            if (reflectiveHit.getMaterial() != nullptr) reflectiveT = reflectiveHit.getT();
            RayTree::AddReflectedSegment(reflectiveRay, 0, reflectiveT);
            color += reflectiveColor * reflectiveResult;
        }

        // refractions
        auto refractiveColor = material->getTransparentColor(hit.getIntersectionPoint());
        if (refractiveColor.Length() > 0) { // refractive material
            auto realNormal = hit.getNormal(), incoming = ray.getDirection();
            auto index_i = indexOfRefraction, index_t = material->getIndexOfRefraction(hit.getIntersectionPoint());
            if (shadeBack && realNormal.Dot3(incoming) > 0) { // intersection inside the object, reverse normal
                realNormal.Negate();
                index_t = 1;
            }
            Vec3f refractiveDir;
            auto hasRefraction = transmittedDirection(realNormal, incoming, index_i, index_t, refractiveDir);
            if (hasRefraction) {
                Ray refractiveRay = {hit.getIntersectionPoint() + ray.getDirection() * 0.001f, refractiveDir};
                Hit refractiveHit;
                auto refractiveResult = traceRay(refractiveRay, 0, bounces + 1,
                                                 weight * reflectiveColor.Length(), index_t, refractiveHit);
                float refractiveT = 1e10;
                if (refractiveHit.getMaterial() != nullptr) refractiveT = refractiveHit.getT();
                RayTree::AddTransmittedSegment(refractiveRay, 0, refractiveT);
                color += refractiveResult * refractiveColor;
            }
        }
    }
    return color;
}

// accelerated ray casting by regular grid
Vec3f RayTracer::traceRayFast(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const {
    assert(grid != nullptr);
    RayTracingStats::IncrementNumNonShadowRays();

    // stop conditions
    // no intersection
    if (!grid->intersect(ray, hit, tmin)) {
        return scene->getBackgroundColor();
    }
    // too many bounces
    if (bounces > maxBounces) {
        return {0, 0, 0};
    }
    // low weight
    if (fcmp(weight - cutoffWeight) < 0) {
        return {0, 0, 0};
    }

    if (weight == 1) { // main segment
        RayTree::SetMainSegment(ray, tmin, hit.getT());
    }

    // ambient
    Vec3f color = scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor(hit.getIntersectionPoint());

    // local shading
    for (int iLight = 0; iLight < scene->getNumLights(); iLight++) {
        // prepare light information
        auto light = scene->getLight(iLight);
        Vec3f dirToLight, lightColor;
        float disToLight;
        light->getIllumination(hit.getIntersectionPoint(), dirToLight, lightColor, disToLight);

        // cast shadow ray and test if blocked
        Ray lightRay = {hit.getIntersectionPoint() - ray.getDirection() * 0.001f, dirToLight};
        Hit lightHit;
        RayTracingStats::IncrementNumShadowRays();
        if (shadows && grid->intersect(lightRay, lightHit, 0) && fcmp(lightHit.getT() - disToLight) < 0) {
            RayTree::AddShadowSegment(lightRay, 0, lightHit.getT());
            continue;
        }
        if (shadows) RayTree::AddShadowSegment(lightRay, 0, disToLight);

        color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
    }

    auto material = dynamic_cast<PhongMaterial *>(hit.getMaterial());
    if (material != nullptr) {
        // reflections
        auto reflectiveColor = material->getReflectiveColor(hit.getIntersectionPoint());
        if (reflectiveColor.Length() > 0) { // reflective material
            Ray reflectiveRay = {hit.getIntersectionPoint() - ray.getDirection() * 0.001f,
                                 mirrorDirection(hit.getNormal(), ray.getDirection())};
            Hit reflectiveHit;
            auto reflectiveResult = traceRay(reflectiveRay, 0, bounces + 1,
                                             weight * reflectiveColor.Length(), indexOfRefraction, reflectiveHit);
            float reflectiveT = 1e10;
            if (reflectiveHit.getMaterial() != nullptr) reflectiveT = reflectiveHit.getT();
            RayTree::AddReflectedSegment(reflectiveRay, 0, reflectiveT);
            color += reflectiveColor * reflectiveResult;
        }

        // refractions
        auto refractiveColor = material->getTransparentColor(hit.getIntersectionPoint());
        if (refractiveColor.Length() > 0) { // refractive material
            auto realNormal = hit.getNormal(), incoming = ray.getDirection();
            auto index_i = indexOfRefraction, index_t = material->getIndexOfRefraction(hit.getIntersectionPoint());
            if (shadeBack && realNormal.Dot3(incoming) > 0) { // intersection inside the object, reverse normal
                realNormal.Negate();
                index_t = 1;
            }
            Vec3f refractiveDir;
            auto hasRefraction = transmittedDirection(realNormal, incoming, index_i, index_t, refractiveDir);
            if (hasRefraction) {
                Ray refractiveRay = {hit.getIntersectionPoint() + ray.getDirection() * 0.001f, refractiveDir};
                Hit refractiveHit;
                auto refractiveResult = traceRay(refractiveRay, 0, bounces + 1,
                                                 weight * reflectiveColor.Length(), index_t, refractiveHit);
                float refractiveT = 1e10;
                if (refractiveHit.getMaterial() != nullptr) refractiveT = refractiveHit.getT();
                RayTree::AddTransmittedSegment(refractiveRay, 0, refractiveT);
                color += refractiveResult * refractiveColor;
            }
        }
    }
    return color;
}
