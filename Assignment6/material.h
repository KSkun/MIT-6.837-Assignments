#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"

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
    virtual Vec3f getDiffuseColor() const { return diffuseColor; }

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

    virtual Vec3f getSpecularColor() const { return specularColor; }

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;

    Vec3f getReflectiveColor() const { return reflectiveColor; }

    Vec3f getTransparentColor() const { return transparentColor; }

    float getIndexOfRefraction() const { return indexOfRefraction; }

protected:
    Vec3f specularColor, reflectiveColor, transparentColor;
    float exponent, indexOfRefraction;
};

class Checkerboard : public Material {
public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2) :
            Material(mat1->getDiffuseColor()), m(m), mat1(mat1), mat2(mat2) {}

    void glSetMaterial() const override {
        mat1->glSetMaterial();
    }

    [[nodiscard]] Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                              const Vec3f &lightColor) const override;

private:
    Matrix *m;
    Material *mat1, *mat2;
};

#endif
