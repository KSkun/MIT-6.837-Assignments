//
// Created by kskun on 2021/10/11.
//

#include <cmath>

#include "global.h"

const float EPSILON = 1e-6;

bool shadeBack = false;
bool gouraud = false;
bool gui = false;
bool shadows = false;
bool visualizeGrid = false;
bool stats = false;
bool randomSamples = false, uniformSamples = false, jitteredSamples = false;
bool boxFilter = false, tentFilter = false, gaussianFilter = false;

int width = 100;
int height = 100;
int thetaSteps = 100, phiSteps = 100;
int maxBounces = 1;
int gridNX = -1, gridNY = -1, gridNZ = -1;
int numSamples = 1;
int sampleZoom = 1;
int filterZoom = 1;

float depthMin = 0;
float depthMax = 1;
float cutoffWeight = 0;
float filterParam;

char *samplesFile = nullptr;
char *filterFile = nullptr;

int fcmp(float f) {
    if (std::fabs(f) < EPSILON) return 0;
    return f < 0 ? -1 : 1;
}