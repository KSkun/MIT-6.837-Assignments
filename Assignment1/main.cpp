#include <iostream>
#include <cassert>
#include <cstring>

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "hit.h"
#include "group.h"
#include "material.h"

int main(int argc, char *argv[]) {
    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-input")) {
            i++;
            assert(i < argc);
            input_file = argv[i];
        } else if (!strcmp(argv[i], "-size")) {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-output")) {
            i++;
            assert(i < argc);
            output_file = argv[i];
        } else if (!strcmp(argv[i], "-depth")) {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    auto parser = new SceneParser(input_file);
    auto camera = parser->getCamera();
    auto group = parser->getGroup();

    const float EPSILON = 1e-8;
    auto colorImg = new Image(width, height);
    colorImg->SetAllPixels(parser->getBackgroundColor());
    auto depthImg = new Image(width, height);
    depthImg->SetAllPixels(Vec3f(0, 0, 0));
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            auto ray = camera->generateRay(Vec2f((float) i / width,
                                                 (float) j / height));
            Hit hit;
            auto interRes = group->intersect(ray, hit, camera->getTMin());
            if (interRes) { // has intersection
                colorImg->SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
                float t = hit.getT();
                if (t > depth_min - EPSILON && t < depth_max + EPSILON) {
                    auto depthCol = (depth_max - t) / (depth_max - depth_min);
                    depthImg->SetPixel(i, j, Vec3f(depthCol, depthCol, depthCol));
                }
            }
        }
    }
    colorImg->SaveTGA(output_file);
    depthImg->SaveTGA(depth_file);

    return 0;
}
