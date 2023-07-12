//
// Created by 29839 on 2023/5/1.
//

#include "DirectionalLight.h"


float DirectionalLight::getIntensity() const {
    return intensity;
}

void DirectionalLight::setIntensity(float intensity) {
    DirectionalLight::intensity = intensity;
}

const Vec3 &DirectionalLight::getColor() const {
    return color;
}

void DirectionalLight::setColor(const Vec3 &color) {
    DirectionalLight::color = color;
}

const Vec3 &DirectionalLight::getDirection() const {
    return direction;
}

void DirectionalLight::setDirection(const Vec3 &direction) {
    DirectionalLight::direction = direction;
}

void DirectionalLight::showControlPanel() {
    ImVec4 color=ImVec4(this->color.x,this->color.y,this->color.z,1.0);
    ImGui::Begin("DirectionalLightController");
    ImGui::SliderFloat("lightStrength",&(this->intensity),0.0f,10.0f);
    ImGui::SliderFloat("LightDir.x",&direction.x,-1.0f,1.0f);
    ImGui::SliderFloat("LightDir.y",&direction.y,-1.0f,1.0f);
    ImGui::SliderFloat("LightDir.z",&direction.z,-1.0f,1.0f);
    ImGui::ColorEdit3("lightColor", (float*)&color);
    this->color.x=color.x;
    this->color.y=color.y;
    this->color.z=color.z;
    ImGui::End();
}

DirectionalLight::DirectionalLight(float intensity, const Vec3 &color, const Vec3 &direction) : intensity(intensity),
                                                                                                color(color),
                                                                                                direction(direction) {}
