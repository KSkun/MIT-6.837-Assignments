#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:
    // CONSTRUCTORS & DESTRUCTOR
    explicit Material(const Vec3f &d_color) { diffuseColor = d_color; }

    virtual ~Material() = default;

    // ACCESSORS
    virtual Vec3f getDiffuseColor(const Vec3f &p) const { return diffuseColor; }

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;

    virtual void glSetMaterial() const = 0;

protected:
    // REPRESENTATION
    Vec3f diffuseColor;
};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material {
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent, const Vec3f &reflectiveColor,
                  const Vec3f &transparentColor, float indexOfRefraction) :
            Material(diffuseColor), specularColor(specularColor), exponent(exponent), reflectiveColor(reflectiveColor),
            transparentColor(transparentColor), indexOfRefraction(indexOfRefraction) {}

    ~PhongMaterial() override = default;

    virtual Vec3f getSpecularColor(const Vec3f &p) const { return specularColor; }

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    virtual Vec3f getReflectiveColor(const Vec3f &p) const { return reflectiveColor; }

    virtual Vec3f getTransparentColor(const Vec3f &p) const { return transparentColor; }

    virtual float getIndexOfRefraction(const Vec3f &p) const { return indexOfRefraction; }

protected:
    Vec3f specularColor, reflectiveColor, transparentColor;
    float exponent, indexOfRefraction;
};

class Checkerboard : public PhongMaterial {
public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2) :
            PhongMaterial(Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0, Vec3f(0, 0, 0), Vec3f(0, 0, 0), 0),
            m(m), mat1(mat1), mat2(mat2) {}

    Checkerboard(Matrix *m, PhongMaterial *mat1, PhongMaterial *mat2) :
            PhongMaterial(*mat1), m(m), mat1(mat1), mat2(mat2) {}

    void glSetMaterial() const override {
        mat1->glSetMaterial();
    }

    [[nodiscard]] Material *getMaterialByPoint(const Vec3f &_p) const {
        Vec3f p = _p;
        m->Transform(p);
        auto px = (int) floor(p.x()), py = (int) floor(p.y()), pz = (int) floor(p.z());
        auto oddCount = (px % 2) + (py % 2) + (pz % 2);
        if (oddCount % 2 == 0) {
            return mat1;
        } else {
            return mat2;
        }
    }

    [[nodiscard]] Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                              const Vec3f &lightColor) const override;

    [[nodiscard]] Vec3f getDiffuseColor(const Vec3f &p) const override {
        auto mat = getMaterialByPoint(p);
        auto pMat = dynamic_cast<PhongMaterial *>(mat);
        if (pMat == nullptr) {
            return {0, 0, 0};
        } else {
            return pMat->getDiffuseColor(p);
        }
    }

    [[nodiscard]] Vec3f getSpecularColor(const Vec3f &p) const override {
        auto mat = getMaterialByPoint(p);
        auto pMat = dynamic_cast<PhongMaterial *>(mat);
        if (pMat == nullptr) {
            return {0, 0, 0};
        } else {
            return pMat->getSpecularColor(p);
        }
    }

    [[nodiscard]] Vec3f getReflectiveColor(const Vec3f &p) const override {
        auto mat = getMaterialByPoint(p);
        auto pMat = dynamic_cast<PhongMaterial *>(mat);
        if (pMat == nullptr) {
            return {0, 0, 0};
        } else {
            return pMat->getReflectiveColor(p);
        }
    }

    [[nodiscard]] Vec3f getTransparentColor(const Vec3f &p) const override {
        auto mat = getMaterialByPoint(p);
        auto pMat = dynamic_cast<PhongMaterial *>(mat);
        if (pMat == nullptr) {
            return {0, 0, 0};
        } else {
            return pMat->getTransparentColor(p);
        }
    }

    [[nodiscard]] float getIndexOfRefraction(const Vec3f &p) const override {
        auto mat = getMaterialByPoint(p);
        auto pMat = dynamic_cast<PhongMaterial *>(mat);
        if (pMat == nullptr) {
            return 0;
        } else {
            return pMat->getIndexOfRefraction(p);
        }
    }

private:
    Matrix *m;
    Material *mat1, *mat2;
};

class Noise : public Material {
public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) :
            Material(mat1->getDiffuseColor({0, 0, 0})), m(m), mat1(mat1), mat2(mat2), octaves(octaves) {}

    void glSetMaterial() const override {
        mat1->glSetMaterial();
    }

    [[nodiscard]] Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                              const Vec3f &lightColor) const override;

private:
    Matrix *m;
    Material *mat1, *mat2;
    int octaves;
};

class Marble : public Material {
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves,
           float frequency, float amplitude) :
            Material(mat1->getDiffuseColor({0, 0, 0})), m(m), mat1(mat1), mat2(mat2), octaves(octaves),
            frequency(frequency), amplitude(amplitude) {}

    void glSetMaterial() const override {
        mat1->glSetMaterial();
    }

    [[nodiscard]] Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                              const Vec3f &lightColor) const override;

private:
    Matrix *m;
    Material *mat1, *mat2;
    int octaves;
    float frequency, amplitude;
};

class Wood : public Material {
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves,
         float frequency, float amplitude) :
            Material(mat1->getDiffuseColor({0, 0, 0})), m(m), mat1(mat1), mat2(mat2), octaves(octaves),
            frequency(frequency), amplitude(amplitude) {}

    void glSetMaterial() const override {
        mat1->glSetMaterial();
    }

    [[nodiscard]] Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                              const Vec3f &lightColor) const override;

private:
    Matrix *m;
    Material *mat1, *mat2;
    int octaves;
    float frequency, amplitude;
};

#endif
