//
// Created by kskun on 2021/12/14.
//

#include "grid.h"

void Grid::paint() {
    auto bMin = bbox->getMin();
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                auto p1 = bMin + Vec3f(i * lx, j * ly, k * lz),
                        p2 = bMin + Vec3f((i + 1) * lx, j * ly, k * lz),
                        p3 = bMin + Vec3f((i + 1) * lx, (j + 1) * ly, k * lz),
                        p4 = bMin + Vec3f(i * lx, (j + 1) * ly, k * lz),
                        p5 = bMin + Vec3f(i * lx, j * ly, (k + 1) * lz),
                        p6 = bMin + Vec3f((i + 1) * lx, j * ly, (k + 1) * lz),
                        p7 = bMin + Vec3f((i + 1) * lx, (j + 1) * ly, (k + 1) * lz),
                        p8 = bMin + Vec3f(i * lx, (j + 1) * ly, (k + 1) * lz);

                glBegin(GL_QUADS);
                glNormal3f(0, 0, -1);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glBegin(GL_QUADS);
                glNormal3f(0, 0, 1);
                glVertex3f(p5.x(), p5.y(), p5.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p7.x(), p7.y(), p7.z());
                glVertex3f(p8.x(), p8.y(), p8.z());

                glBegin(GL_QUADS);
                glNormal3f(0, -1, 0);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p5.x(), p5.y(), p5.z());
                glBegin(GL_QUADS);
                glNormal3f(0, 1, 0);
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glVertex3f(p8.x(), p8.y(), p8.z());
                glVertex3f(p7.x(), p7.y(), p7.z());

                glBegin(GL_QUADS);
                glNormal3f(-1, 0, 0);
                glVertex3f(p1.x(), p1.y(), p1.z());
                glVertex3f(p4.x(), p4.y(), p4.z());
                glVertex3f(p8.x(), p8.y(), p8.z());
                glVertex3f(p5.x(), p5.y(), p5.z());
                glBegin(GL_QUADS);
                glNormal3f(1, 0, 0);
                glVertex3f(p2.x(), p2.y(), p2.z());
                glVertex3f(p3.x(), p3.y(), p3.z());
                glVertex3f(p6.x(), p6.y(), p6.z());
                glVertex3f(p7.x(), p7.y(), p7.z());
            }
        }
    }
}