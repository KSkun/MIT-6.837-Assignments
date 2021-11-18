//
// Created by kskun on 2021/10/11.
//

#include "renderer.h"
#include "global.h"
#include "camera.h"
#include "group.h"
#include "material.h"
#include "light.h"

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
            Vec3f color = Vec3f(0, 0, 0);
            auto diffuseCol = hit.getMaterial()->getDiffuseColor();

            for (int iLight = 0; iLight < scene->getNumLights(); iLight++) {
                auto light = scene->getLight(iLight);
                Vec3f lightDir, lightCol;
                light->getIllumination(hit.getIntersectionPoint(), lightDir, lightCol);

                auto n = hit.getNormal();
                // shade back
                if (shadeBack && ray.getDirection().Dot3(n) > -EPSILON) {
                    n.Scale(-1, -1, -1);
                }

                auto d = max(lightDir.Dot3(n), 0.0f);
                lightCol.Scale(d * diffuseCol.x(), d * diffuseCol.y(), d * diffuseCol.z());
                color += lightCol;
            }
            auto ambientCol = scene->getAmbientLight();
            ambientCol.Scale(diffuseCol.x(), diffuseCol.y(), diffuseCol.z());
            color += ambientCol;
            color.Clamp();

            image->SetPixel(i, j, color);
        }
    }
}