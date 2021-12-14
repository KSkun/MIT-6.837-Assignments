//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_GLOBAL_H
#define ASSIGNMENT2_GLOBAL_H

extern const float EPSILON;

extern bool shadeBack;
extern bool gouraud;
extern bool gui;
extern bool shadows;

extern int width;
extern int height;
extern int thetaSteps, phiSteps;
extern int maxBounces;

extern float depthMin;
extern float depthMax;
extern float cutoffWeight;

int fcmp(float f);

#endif //ASSIGNMENT2_GLOBAL_H
