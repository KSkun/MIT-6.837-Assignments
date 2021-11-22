//
// Created by kskun on 2021/10/11.
//

#ifndef ASSIGNMENT2_RENDERER_H
#define ASSIGNMENT2_RENDERER_H


#include "image.h"
#include "scene_parser.h"
#include "hit.h"

class Renderer {
public:
    explicit Renderer(Image *image = nullptr, SceneParser *scene = nullptr) : image(image),
                                                                              scene(scene) {
        assert(scene);
        camera = scene->getCamera();
        group = scene->getGroup();
        width = image->Width();
        height = image->Height();
    }

    virtual void Render() {
        assert(image);
        image->SetAllPixels(scene->getBackgroundColor());
    };

protected:
    Image *image;
    SceneParser *scene;
    Camera *camera;
    Group *group;
    int width, height;
};

class DepthRenderer : public Renderer {
public:
    explicit DepthRenderer(Image *image = nullptr, SceneParser *scene = nullptr,
                           float depthMin = -std::numeric_limits<float>::infinity(),
                           float depthMax = std::numeric_limits<float>::infinity()) :
            Renderer(image, scene), depthMin(depthMin), depthMax(depthMax) {}

    void Render() override;

protected:
    float depthMin, depthMax;
};

class ColorRenderer : public Renderer {
public:
    explicit ColorRenderer(Image *image = nullptr, SceneParser *scene = nullptr) :
            Renderer(image, scene) {}

    void Render() override;
};

class NormalRenderer : public Renderer {
public:
    explicit NormalRenderer(Image *image = nullptr, SceneParser *scene = nullptr) :
            Renderer(image, scene) {}

    void Render() override;
};

class DiffuseRenderer : public Renderer {
public:
    explicit DiffuseRenderer(Image *image = nullptr, SceneParser *scene = nullptr) :
            Renderer(image, scene) {}

    void Render() override;
};

#endif //ASSIGNMENT2_RENDERER_H
