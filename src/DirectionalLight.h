//
// Created by 29839 on 2023/5/1.
//

#ifndef RACINGCAR_DIRECTIONALLIGHT_H
#define RACINGCAR_DIRECTIONALLIGHT_H

#include "Base.h"
#include "imgui/imgui.h"
class DirectionalLight {
public:
    float intensity;
    Vec3 color;
    Vec3 direction;
    void showControlPanel();
    DirectionalLight(float intensity, const Vec3 &color, const Vec3 &direction);

    float getIntensity() const;

    void setIntensity(float intensity);

    const Vec3 &getColor() const;

    void setColor(const Vec3 &color);

    const Vec3 &getDirection() const;

    void setDirection(const Vec3 &direction);
};


#endif //RACINGCAR_DIRECTIONALLIGHT_H
