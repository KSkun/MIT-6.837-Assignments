//
// Created by kskun on 2021/12/14.
//

#ifndef ASSIGNMENT5_GRID_H
#define ASSIGNMENT5_GRID_H

#include <tuple>
#include <utility>
#include <algorithm>
#include <array>

#include "object3d.h"
#include "object3dvector.h"

class MarchingInfo;

class Grid : public Object3D {
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz) :
            Object3D(nullptr, new BoundingBox(*bb)), nx(nx), ny(ny), nz(nz) {
        assert(bb != nullptr);

        auto bMax = bb->getMax(), bMin = bb->getMin();
        lx = (float) (bMax.x() - bMin.x()) / nx;
        ly = (float) (bMax.y() - bMin.y()) / ny;
        lz = (float) (bMax.z() - bMin.z()) / nz;

//        isOpaque = new bool[nx * ny * nz];
//        std::fill(isOpaque, isOpaque + nx * ny * nz, false);
        objects = new Object3DVector[nx * ny * nz];
    }

    ~Grid() override {
//        delete[] isOpaque;
        delete[] objects;
    }

    void insertObject(int x, int y, int z, Object3D *obj) {
        assert(x >= 0 && x < nx);
        assert(y >= 0 && y < ny);
        assert(z >= 0 && z < nz);
        objects[x * ny * nz + y * nz + z].addObject(obj);
    }

    [[nodiscard]] Object3DVector *getObjects(int x, int y, int z) const {
        assert(x >= 0 && x < nx);
        assert(y >= 0 && y < ny);
        assert(z >= 0 && z < nz);
        return &objects[x * ny * nz + y * nz + z];
    }

    [[nodiscard]] bool occupied(int x, int y, int z) const {
        assert(x >= 0 && x < nx);
        assert(y >= 0 && y < ny);
        assert(z >= 0 && z < nz);
        return objects[x * ny * nz + y * nz + z].getNumObjects() > 0;
    }

    [[nodiscard]] bool occupied(const std::tuple<int, int, int> &index) const {
        auto[x, y, z] = index;
        return occupied(x, y, z);
    }

    [[nodiscard]] std::tuple<int, int, int> getSize() const {
        return {nx, ny, nz};
    }

    [[nodiscard]] std::tuple<float, float, float> getVoxelSize() const {
        return {lx, ly, lz};
    }

    [[nodiscard]] Vec3f getMin() const {
        return bbox->getMin();
    }

    [[nodiscard]] Vec3f getMax() const {
        return bbox->getMax();
    }

    bool intersect(const Ray &r, Hit &h, float tMin) override;

    void paint() override;

    int initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

protected:
    int nx, ny, nz;
    float lx, ly, lz;
//    bool *isOpaque;
    Object3DVector *objects;

    void hitFace(const BoundingBox *bbox, const Vec3f &inter, const MarchingInfo &mi, const int ret,
                 Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4, Vec3f &n) const;
};

class MarchingInfo {
public:
    float tMin;
    int i, j, k;
    float tNextX, tNextY, tNextZ;
    float dTX, dTY, dTZ;
    int signX, signY, signZ;
    bool valid;
    const Grid *grid;

    int nextCell();
};


#endif //ASSIGNMENT5_GRID_H
