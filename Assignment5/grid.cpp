//
// Created by kskun on 2021/12/14.
//

#include <algorithm>

#include "grid.h"
#include "global.h"
#include "rayTree.h"

void Grid::paint() {
    auto bMin = bbox->getMin();
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                // skip empty voxels
                if (!occupied(i, j, k)) continue;

                auto[p1, p2, p3, p4,
                p5, p6, p7, p8] = getVoxelVertices(i, j, k);

                glBegin(GL_QUADS);

                if (k == 0 || !occupied(i, j, k - 1)) {
                    glNormal3f(0, 0, -1);
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                }
                if (k == nz - 1 || !occupied(i, j, k + 1)) {
                    glNormal3f(0, 0, 1);
                    glVertex3f(p5.x(), p5.y(), p5.z());
                    glVertex3f(p6.x(), p6.y(), p6.z());
                    glVertex3f(p7.x(), p7.y(), p7.z());
                    glVertex3f(p8.x(), p8.y(), p8.z());
                }

                if (j == 0 || !occupied(i, j - 1, k)) {
                    glNormal3f(0, -1, 0);
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glVertex3f(p6.x(), p6.y(), p6.z());
                    glVertex3f(p5.x(), p5.y(), p5.z());
                }
                if (j == ny - 1 || !occupied(i, j + 1, k)) {
                    glNormal3f(0, 1, 0);
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                    glVertex3f(p8.x(), p8.y(), p8.z());
                    glVertex3f(p7.x(), p7.y(), p7.z());
                }

                if (i == 0 || !occupied(i - 1, j, k)) {
                    glNormal3f(-1, 0, 0);
                    glVertex3f(p1.x(), p1.y(), p1.z());
                    glVertex3f(p4.x(), p4.y(), p4.z());
                    glVertex3f(p8.x(), p8.y(), p8.z());
                    glVertex3f(p5.x(), p5.y(), p5.z());
                }
                if (i == nx - 1 || !occupied(i + 1, j, k)) {
                    glNormal3f(1, 0, 0);
                    glVertex3f(p2.x(), p2.y(), p2.z());
                    glVertex3f(p3.x(), p3.y(), p3.z());
                    glVertex3f(p7.x(), p7.y(), p7.z());
                    glVertex3f(p6.x(), p6.y(), p6.z());
                }

                glEnd();
            }
        }
    }
}

int Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tMin) const {
    mi.grid = this;

    const auto &Rd = r.getDirection(), &Ro = r.getOrigin(),
            &pMin = bbox->getMin(), &pMax = bbox->getMax();
    // intersect with bounding box
    // parallel no intersection
    for (int i = 0; i < 3; i++) {
        if (Rd[i] == 0 && (Ro[i] < pMin[i] || Ro[i] > pMax[i])) {
            mi.valid = false;
            return -1;
        }
    }

    // calculate tNear & tFar
    float tNear, tFar;
    int dNear;
    for (int i = 0; i < 3; i++) {
        float t1 = (pMin[i] - Ro[i]) / Rd[i],
                t2 = (pMax[i] - Ro[i]) / Rd[i];
        if (t1 > t2) std::swap(t1, t2);
        if (i == 0 || t1 > tNear) {
            tNear = t1;
            dNear = i;
        }
        if (i == 0 || t2 < tFar) {
            tFar = t2;
        }
    }
    // miss bounding box
    if (tNear > tFar || tFar < tMin) {
        mi.valid = false;
        return -1;
    }

    // intersect with bounding box
    mi.valid = true;

    mi.dTX = std::abs(lx / Rd.x());
    mi.dTY = std::abs(ly / Rd.y());
    mi.dTZ = std::abs(lz / Rd.z());

    mi.signX = Rd.x() < 0 ? -1 : 1;
    mi.signY = Rd.y() < 0 ? -1 : 1;
    mi.signZ = Rd.z() < 0 ? -1 : 1;

    int ret = -1;
    if (tNear > tMin) {
        // origin outside grid
        mi.tMin = tNear;
        ret = dNear;
    } else {
        // origin inside grid
        mi.tMin = tMin;
    }

    // grid index
    auto p = r.pointAtParameter(mi.tMin),
            pRelative = p - pMin + Vec3f(EPSILON, EPSILON, EPSILON);
    mi.i = std::min((int) (pRelative.x() / lx), nx - 1);
    mi.j = std::min((int) (pRelative.y() / ly), ny - 1);
    mi.k = std::min((int) (pRelative.z() / lz), nz - 1);

    // next grid index
    mi.tNextX = ((mi.i + (mi.signX < 0 ? 0 : 1)) * lx + pMin.x() - Ro.x()) / Rd.x();
    mi.tNextY = ((mi.j + (mi.signY < 0 ? 0 : 1)) * ly + pMin.y() - Ro.y()) / Rd.y();
    mi.tNextZ = ((mi.k + (mi.signZ < 0 ? 0 : 1)) * lz + pMin.z() - Ro.z()) / Rd.z();
    return ret;
}

int MarchingInfo::nextCell() {
    int ret;
    // 3DDDA
    assert(valid);
    tMin = std::min({tNextX, tNextY, tNextZ});

    if (tMin == tNextX) {
        tNextX += dTX;
        i += signX;
        ret = 0;
    } else if (tMin == tNextY) {
        tNextY += dTY;
        j += signY;
        ret = 1;
    } else if (tMin == tNextZ) {
        tNextZ += dTZ;
        k += signZ;
        ret = 2;
    } else {
        assert(false);
        ret = -1;
    }

    auto[nx, ny, nz] = grid->getSize();
    if ((i < 0 || i >= nx) || (j < 0 || j >= ny) || (k < 0 || k >= nz)) {
        valid = false;
    }
    return ret;
}

void Grid::hitFace(const BoundingBox *bbox, const Vec3f &inter, const MarchingInfo &mi, const int ret,
                   Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4, Vec3f &n) const {
    auto p = inter - bbox->getMin();
    auto[i, j, k] = std::array{mi.i, mi.j, mi.k};
    if (ret == 0) {
        if (mi.signX < 0) i++;
        p1 = bbox->getMin() + Vec3f(lx * i, ly * j, lz * k);
        p2 = bbox->getMin() + Vec3f(lx * i, ly * (j + 1), lz * k);
        p3 = bbox->getMin() + Vec3f(lx * i, ly * j, lz * (k + 1));
        p4 = bbox->getMin() + Vec3f(lx * i, ly * (j + 1), lz * (k + 1));
        n.Set(1, 0, 0);
    } else if (ret == 1) {
        if (mi.signY < 0) j++;
        p1 = bbox->getMin() + Vec3f(lx * i, ly * j, lz * k);
        p2 = bbox->getMin() + Vec3f(lx * (i + 1), ly * j, lz * k);
        p3 = bbox->getMin() + Vec3f(lx * i, ly * j, lz * (k + 1));
        p4 = bbox->getMin() + Vec3f(lx * (i + 1), ly * j, lz * (k + 1));
        n.Set(0, 1, 0);
    } else if (ret == 2) {
        if (mi.signZ < 0) k++;
        p1 = bbox->getMin() + Vec3f(lx * i, ly * j, lz * k);
        p2 = bbox->getMin() + Vec3f(lx * (i + 1), ly * j, lz * k);
        p3 = bbox->getMin() + Vec3f(lx * i, ly * (j + 1), lz * k);
        p4 = bbox->getMin() + Vec3f(lx * (i + 1), ly * (j + 1), lz * k);
        n.Set(0, 0, 1);
    } else {
        assert(false);
    }
}

bool Grid::intersect(const Ray &r, Hit &h, float tMin) {
    static Material *m = new PhongMaterial(
            {1, 1, 1}, {0, 0, 0}, 1,
            {0, 0, 0}, {0, 0, 0}, 1);
    MarchingInfo mi;
    int ret = initializeRayMarch(mi, r, tMin);
    assert(ret != -1);

    while (mi.valid && !occupied(mi.i, mi.j, mi.k)) {
        auto p = r.pointAtParameter(mi.tMin);
        Vec3f p1, p2, p3, p4, n;
        hitFace(bbox, p, mi, ret, p1, p2, p3, p4, n);
        if (n.Dot3(r.getDirection()) > 0) n.Negate();
        RayTree::AddEnteredFace(p1, p2, p4, p3, n, m);

        ret = mi.nextCell();
        assert(ret != -1);
    }
    if (mi.valid) {
        auto p = r.pointAtParameter(mi.tMin);
        Vec3f p1, p2, p3, p4, n;
        hitFace(bbox, p, mi, ret, p1, p2, p3, p4, n);
        if (n.Dot3(r.getDirection()) > 0) n.Negate();
        RayTree::AddHitCellFace(p1, p2, p4, p3, n, m);

        h.set(mi.tMin, m, n, r);
        return true;
    }
    return false;
}