//
// Created by kskun on 2021/10/11.
//

#include <cmath>

#include "global.h"

const float EPSILON = 1e-12;

bool shadeBack = false;
bool gouraud = false;
bool gui = false;
bool shadows = false;

int width = 100;
int height = 100;
int thetaSteps = 100, phiSteps = 100;
int maxBounces = 1;

float depthMin = 0;
float depthMax = 1;
float cutoffWeight = 0;

int fcmp(float f) {
    if (std::fabs(f) < EPSILON) return 0;
    return f < 0 ? -1 : 1;
}