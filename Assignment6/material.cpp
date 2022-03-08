//
// Created by kskun on 2021/11/21.
//

#include <windows.h>
#include <gl/GL.h>

#include "material.h"
#include "matrix.h"
#include "perlin_noise.h"

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    Vec3f n = hit.getNormal();
    Vec3f l = dirToLight;
    l.Normalize();
    Vec3f Ld = diffuseColor * n.Dot3(l) * lightColor;

    Vec3f v = ray.getDirection();
    v.Scale(-1, -1, -1); // reverse direction
    Vec3f h = l + v; // half way vector
    h.Normalize();
    Vec3f Ls = specularColor * pow(n.Dot3(h), exponent) * lightColor;

    return Ld + Ls;
}

void PhongMaterial::glSetMaterial() const {

    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
            specularColor.r(),
            specularColor.g(),
            specularColor.b(),
            1.0};
    GLfloat diffuse[4] = {
            diffuseColor.r(),
            diffuseColor.g(),
            diffuseColor.b(),
            1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0) glexponent = 0;
    if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}

Vec3f Checkerboard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    return getMaterialByPoint(hit.getIntersectionPoint())->Shade(ray, hit, dirToLight, lightColor);
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    auto p = hit.getIntersectionPoint();
    m->Transform(p);
    float N = 0;
    for (int i = 0; i < octaves; i++) {
        float oct = powf(2, i);
        N += PerlinNoise::noise(p.x() * oct, p.y() * oct, p.z() * oct) / oct;
    }

    auto color1 = mat1->Shade(ray, hit, dirToLight, lightColor),
            color2 = mat2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * N + color2 * (1 - N);
}

Vec3f Marble::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    auto p = hit.getIntersectionPoint();
    m->Transform(p);
    float N = 0;
    for (int i = 0; i < octaves; i++) {
        float oct = powf(2, i);
        N += PerlinNoise::noise(p.x() * oct, p.y() * oct, p.z() * oct) / oct;
    }
    float M = sin(frequency * p.x() + amplitude * N);
    M = (M + 1) / 2; // map [-1, 1] to [0, 1]

    auto color1 = mat1->Shade(ray, hit, dirToLight, lightColor),
            color2 = mat2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * M + color2 * (1 - M);
}

Vec3f Wood::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
    auto p = hit.getIntersectionPoint();
    m->Transform(p);
    float N = 0;
    for (int i = 0; i < octaves; i++) {
        float oct = powf(2, i);
        N += PerlinNoise::noise(p.x() * oct, p.y() * oct, p.z() * oct) / oct;
    }
    float M = sin(frequency * sqrt(p.x() * p.x() + p.y() * p.y()) + amplitude * N);
    M = (M + 1) / 2; // map [-1, 1] to [0, 1]

    auto color1 = mat1->Shade(ray, hit, dirToLight, lightColor),
            color2 = mat2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * M + color2 * (1 - M);
}
