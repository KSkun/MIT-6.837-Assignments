//
// Created by kskun on 2021/12/14.
//

#ifndef ASSIGNMENT5_GRID_H
#define ASSIGNMENT5_GRID_H

#include <tuple>

#include "object3d.h"


class MarchingInfo {
public:
    float tMin;
    int i, j, k;
    float tNextX, tNextY, tNextZ;
    float dTX, dTY, dTZ;
    int signX, signY, signZ;
    bool next;
    const Grid *grid;

    void nextCell();
};

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz) :
            Object3D(nullptr, bb), nx(nx), ny(ny), nz(nz) {
        assert(bb != nullptr);

        auto bMax = bb->getMax(), bMin = bb->getMin();
        lx = (float) (bMax.x() - bMin.x()) / nx;
        ly = (float) (bMax.y() - bMin.y()) / ny;
        lz = (float) (bMax.z() - bMin.z()) / nz;

        isOpaque = new bool[nx * ny * nz];
        std::fill(isOpaque, isOpaque + nx * ny * nz, false);
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

    [[nodiscard]] bool getOpaqueness(int x, int y, int z) const {
        return isOpaque[x * ny * nz + y * nz + z];
    }

    [[nodiscard]] std::tuple<int, int, int> getSize() const {
        return {nx, ny, nz};
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

protected:
    int nx, ny, nz;
    float lx, ly, lz;
    bool *isOpaque;

    void hitFace(const BoundingBox *bbox, const Vec3f &inter, const MarchingInfo &mi,
                 Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4, Vec3f &n) const;
};


#endif //ASSIGNMENT5_GRID_H
