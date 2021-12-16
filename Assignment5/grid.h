//
// Created by kskun on 2021/12/14.
//

#ifndef ASSIGNMENT5_GRID_H
#define ASSIGNMENT5_GRID_H

#include <tuple>

#include "object3d.h"

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz) :
            Object3D(nullptr, bb), nx(nx), ny(ny), nz(nz) {
        assert(bb != nullptr);
        isOpaque = new bool[nx * ny * nz];
    }

    ~Grid() override {
        delete[] isOpaque;
        Object3D::~Object3D();
    }

    void setOpaqueness(int x, int y, int z, bool opaqueness) {
        assert(x >= 0 && x < nx);
        assert(y >= 0 && y < ny);
        assert(z >= 0 && z < nz);
        assert(x * ny * nz + y * nz + z < nx * ny * nz);
        isOpaque[x * ny * nz + y * nz + z] = opaqueness;
    }

    bool getOpaqueness(int x, int y, int z) {
        return isOpaque[x * ny * nz + y * nz + z];
    }

    std::tuple<int, int, int> getSize() {
        return {nx, ny, nz};
    }

protected:
    int nx, ny, nz;
    bool *isOpaque;
};


#endif //ASSIGNMENT5_GRID_H
