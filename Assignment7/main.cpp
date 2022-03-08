#include <iostream>
#include <cassert>
#include <cstring>

#include <windows.h>
#include <GL/glut.h>

#include "scene_parser.h"
#include "image.h"
#include "camera.h"
#include "hit.h"
#include "group.h"
#include "renderer.h"
#include "global.h"
#include "ray_tracer.h"

SceneParser *parser;
RayTracer *tracer;
Grid *grid;
char *input_file = nullptr;
char *output_file = nullptr;
char *normals_file = nullptr;
char *depth_file = nullptr;

void Render() {
    if (output_file) {
        auto colorImg = new Image(width, height);
        auto colorRenderer = new RayTraceRenderer(colorImg, parser);
        colorRenderer->Render();
        colorImg->SaveTGA(output_file);
    }
//    if (depth_file) {
//        auto depthImg = new Image(width, height);
//        auto depthRenderer = new DepthRenderer(depthImg, parser, depthMin, depthMax);
//        depthRenderer->Render();
//        depthImg->SaveTGA(depth_file);
//    }
//    if (normals_file) {
//        auto normalsFile = new Image(width, height);
//        auto normalsRenderer = new NormalRenderer(normalsFile, parser);
//        normalsRenderer->Render();
//        normalsFile->SaveTGA(normals_file);
//    }
}

void TraceRay(float x, float y) {
    auto camera = parser->getCamera();
    auto ray = camera->generateRay({x, y});
    Hit hit;
    if (gridNX != -1) {
        // grid accelerated ray tracing
        tracer->traceRayFast(ray, camera->getTMin(), 0, 1, 1, hit);
    } else {
        tracer->traceRay(ray, camera->getTMin(), 0, 1, 1, hit);
    }
}

int main(int argc, char *argv[]) {
    // process command line parameters
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
            depthMin = atof(argv[i]);
            i++;
            assert(i < argc);
            depthMax = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        } else if (!strcmp(argv[i], "-shade_back")) {
            shadeBack = true;
        } else if (!strcmp(argv[i], "-gui")) {
            gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++;
            assert(i < argc);
            thetaSteps = atoi(argv[i]);
            i++;
            assert(i < argc);
            phiSteps = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++;
            assert(i < argc);
            maxBounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++;
            assert(i < argc);
            cutoffWeight = atof(argv[i]);
        } else if (!strcmp(argv[i], "-grid")) {
            i++;
            assert(i < argc);
            gridNX = atoi(argv[i]);
            i++;
            assert(i < argc);
            gridNY = atoi(argv[i]);
            i++;
            assert(i < argc);
            gridNZ = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-visualize_grid")) {
            visualizeGrid = true;
        } else if (!strcmp(argv[i], "-stats")) {
            stats = true;
        } else {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    parser = new SceneParser(input_file);
    tracer = new RayTracer(parser, maxBounces, cutoffWeight);
    if (gui) {
        glutInit(&argc, argv);
        auto *glCanvas = new GLCanvas;
        glCanvas->initialize(parser, Render, TraceRay,
                             tracer->getGrid(), visualizeGrid);
    } else {
        Render();
    }

    return 0;
}
