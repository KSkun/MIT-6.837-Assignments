//
// Created by kskun on 2021/10/3.
//

#include "ifs.h"

IFS::IFS() {
    n = 0;
    trans = nullptr;
    prob = nullptr;
    rand = new Random;
}

void IFS::InputTrans(const char *file) {
    assert(file);
    FILE *input = fopen(file, "r");
    assert(input != nullptr);

    // read the number of transforms
    int num_transforms;
    fscanf(input, "%d", &num_transforms);

    n = num_transforms;
    assert(n > 0);
    delete[] trans;
    delete[] prob;
    trans = new Matrix[n];
    prob = new float[n];

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) {
        float probability;
        fscanf(input, "%f", &probability);
        Matrix m;
        m.Read3x3(input);
        trans[i] = m;
        prob[i] = probability;
    }

    // close the file
    fclose(input);
}

void IFS::Render(Image *image, int points, int iters) {
    assert(n > 0);
    assert(image);
    int size = image->Height();

    for (int i = 0; i < points; i++) {
        Vec2f p = {rand->randFloatUniform(), rand->randFloatUniform()};
        for (int k = 0; k < iters; k++) {
            auto p1 = rand->randFloatUniform();
            auto p2 = 0.0f;
            Matrix *m;
            for (int tIdx = 0; tIdx < n; tIdx++) {
                p2 += prob[tIdx];
                if (p1 < p2) {
                    m = &trans[tIdx];
                    break;
                }
            }
            m->Transform(p);
        }
        image->SetPixel(p.x() * size, p.y() * size, Vec3f(0, 0, 0));
    }
}

IFS::~IFS() {
    delete[] trans;
    delete[] prob;
    delete rand;
}