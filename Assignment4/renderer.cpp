//
// Created by kskun on 2021/10/11.
//

#include "renderer.h"
#include "global.h"
#include "camera.h"
#include "group.h"
#include "material.h"
#include "light.h"
#include "ray_tracer.h"

void DepthRenderer::Render() {
    assert(image);
    image->SetAllPixels(Vec3f(0, 0, 0));

    for (int i = 0; i < image->Width(); i++) {
        for (int j = 0; j < image->Height(); j++) {
            auto ray = camera->generateRay(Vec2f((float) i / width,
                                                 (float) j / height));
            Hit hit;
            auto interRes = group->intersect(ray, hit, camera->getTMin());
            if (!interRes) continue;
            // has intersection
            float t = hit.getT();
            if (t > depthMin - EPSILON && t < depthMax + EPSILON) {
                auto depthCol = (depthMax - t) / (depthMax - depthMin);
                image->SetPixel(i, j, Vec3f(depthCol, depthCol, depthCol));
            }
        }
    }
}

void ColorRenderer::Render() {
    Renderer::Render(); // preparations

    for (int i = 0; i < image->Width(); i++) {
        for (int j = 0; j < image->Height(); j++) {
            auto ray = camera->generateRay(Vec2f((float) i / image->Width(),
                                                 (float) j / image->Height()));
            Hit hit;
            auto interRes = scene->getGroup()->intersect(ray, hit, camera->getTMin());
            if (!interRes) continue;
            // has intersection
            image->SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
        }
    }
}

void NormalRenderer::Render() {
    assert(image);
    image->SetAllPixels(Vec3f(0, 0, 0));

    for (int i = 0; i < image->Width(); i++) {
        for (int j = 0; j < image->Height(); j++) {
            auto ray = camera->generateRay(Vec2f((float) i / image->Width(),
                                                 (float) j / image->Height()));
            Hit hit;
            auto interRes = scene->getGroup()->intersect(ray, hit, camera->getTMin());
            if (!interRes) continue;
            // has intersection
            auto n = hit.getNormal();
            n.Set(fabs(n.x()), fabs(n.y()), fabs(n.z()));
            image->SetPixel(i, j, n);
        }
    }
}

void DiffuseRenderer::Render() {
    Renderer::Render(); // preparations

    for (int i = 0; i < image->Width(); i++) {
        for (int j = 0; j < image->Height(); j++) {
            auto ray = camera->generateRay(Vec2f((float) i / image->Width(),
                                                 (float) j / image->Height()));
            Hit hit;
            auto interRes = scene->getGroup()->intersect(ray, hit, camera->getTMin());
            if (!interRes) continue;
            // has intersection
            auto material = hit.getMaterial();
            Vec3f color = Vec3f(0, 0, 0);
            for (int iLight = 0; iLight < scene->getNumLights(); iLight++) {
                auto light = scene->getLight(iLight);
                Vec3f lightDir, lightCol;
                float distanceToLight;
                light->getIllumination(hit.getIntersectionPoint(), lightDir, lightCol, distanceToLight);
                color += material->Shade(ray, hit, lightDir, lightCol);
            }
            color += scene->getAmbientLight() * material->getDiffuseColor();
            image->SetPixel(i, j, color);
        }
    }
}

void RayTraceRenderer::Render() {
    Renderer::Render(); // preparations

    for (int i = 0; i < image->Width(); i++) {
        for (int j = 0; j < image->Height(); j++) {
            auto ray = camera->generateRay(Vec2f((float) i / image->Width(),
                                                 (float) j / image->Height()));
            auto tracer = new RayTracer(scene, maxBounces, cutoffWeight);
            Hit hit;
            image->SetPixel(i, j, tracer->traceRay(ray, camera->getTMin(), 0, 1, 1, hit));
        }
    }
}