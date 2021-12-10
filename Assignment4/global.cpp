//
// Created by kskun on 2021/10/11.
//

#include "global.h"

const float EPSILON = 1e-6;

bool shadeBack = false;
bool gouraud = false;
bool gui = false;
bool shadows = false;

int width = 100;
int height = 100;
int thetaSteps = 100, phiSteps = 100;
int bounces = 1;

float depthMin = 0;
float depthMax = 1;
float weight = 0;