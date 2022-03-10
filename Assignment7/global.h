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
extern bool visualizeGrid;
extern bool stats;
extern bool randomSamples, uniformSamples, jitteredSamples;
extern bool boxFilter, tentFilter, gaussianFilter;

extern int width;
extern int height;
extern int thetaSteps, phiSteps;
extern int maxBounces;
extern int gridNX, gridNY, gridNZ;
extern int numSamples;
extern int sampleZoom;
extern int filterZoom;

extern float depthMin;
extern float depthMax;
extern float cutoffWeight;
extern float filterParam;

extern char *samplesFile;
extern char *filterFile;

int fcmp(float f);

#endif //ASSIGNMENT2_GLOBAL_H
