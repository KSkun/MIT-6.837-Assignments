#include <iostream>
#include <cassert>
#include <cstring>

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "hit.h"
#include "group.h"
#include "material.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
    char *input_file = nullptr;
    int width = 100;
    int height = 100;
    char *output_file = nullptr;
    char *normals_file = nullptr;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = nullptr;
    bool shade_back = false;

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
        } else if (!strcmp(argv[i], "-normals")) {
            i++;
            assert(i < argc);
            normals_file = argv[i];
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
        } else if (!strcmp(argv[i], "-shade_back")) {
            shade_back = true;
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    auto parser = new SceneParser(input_file);
    if (output_file) {
        auto colorImg = new Image(width, height);
        auto colorRenderer = new DiffuseRenderer(colorImg, parser, shade_back);
        colorRenderer->Render();
        colorImg->SaveTGA(output_file);
    }
    if (depth_file) {
        auto depthImg = new Image(width, height);
        auto depthRenderer = new DepthRenderer(depthImg, parser, depth_min, depth_max);
        depthRenderer->Render();
        depthImg->SaveTGA(depth_file);
    }
    if (normals_file) {
        auto normalsFile = new Image(width, height);
        auto normalsRenderer = new NormalRenderer(normalsFile, parser);
        normalsRenderer->Render();
        normalsFile->SaveTGA(normals_file);
    }

    return 0;
}
