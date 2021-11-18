//
// Created by kskun on 2021/10/3.
//

#ifndef ASSIGNMENT0_IFS_H
#define ASSIGNMENT0_IFS_H

#include <vector>

#include "matrix.h"
#include "image.h"
#include "random.h"

class IFS {
public:
    IFS();

    void InputTrans(const char *file);

    void Render(Image *image, int points, int iters);

    ~IFS();

private:
    int n;
    Matrix* trans;
    float* prob;
    Random *rand;
};

#endif //ASSIGNMENT0_IFS_H
