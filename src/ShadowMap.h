//
// Created by 29839 on 2023/5/1.
//

#ifndef RACINGCAR_SHADOWMAP_H
#define RACINGCAR_SHADOWMAP_H

#include "DirectionalLight.h"


class ShadowMap {
private:
    GLuint depthTexture;
    GLuint depthFBO;
    float zFar;
    float zNear;
    float orthSize;
    Shared<DirectionalLight> light;

    Shared<Shader> depthShader;

    Mat4 lightSpaceMatrix;
    unsigned int shadowWidth;
    unsigned int shadowHeight;
public:
    ShadowMap(){
        zFar = 20.0f;
        zNear = 0.0f;
        orthSize = 200.0f;
    }
    void initShadowMap(Shared<DirectionalLight> light,int width,int height){
        shadowWidth = (unsigned int)width;
        shadowHeight = (unsigned int)height;
        depthShader = createShared<Shader>("../Shader/depthVertex.glsl","../Shader/depthFragment.glsl");
        this->light = light;
        glGenTextures(1,&depthTexture);
        glBindTexture(GL_TEXTURE_2D,depthTexture);
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,shadowWidth,shadowHeight,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glGenFramebuffers(1,&depthFBO);
        glBindFramebuffer(GL_FRAMEBUFFER,depthFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture,0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }
    void drawDepthMap(){
        glViewport(0, 0, shadowWidth, shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER,depthFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

    }
    void loadData(const Vec3& pos){
        Mat4 lightProjection =
                glm::ortho(-orthSize,orthSize,-orthSize,orthSize,zNear,zFar);
        Mat4 lightView =
                glm::lookAt(pos-3.0f*normalize(light->getDirection()),
                            pos,
                            Vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix =lightProjection*lightView;
        //RenderScene();
        depthShader->bind();
        depthShader->setMat4("lightSpaceMatrix",lightSpaceMatrix);
    }
    const Mat4 &getLightSpaceMatrix() const {
        return lightSpaceMatrix;
    }
    Shared<Shader> getDepthShader()const{
        return this->depthShader;
    }
    const Shared<DirectionalLight> &getLight() const {
        return light;
    }
    const float& getTextureSize(){
        return float(shadowWidth);
    }

    void setLight(const Shared<DirectionalLight> &light) {
        ShadowMap::light = light;
    }

    void setDepthShader(const Shared<Shader> &depthShader) {
        ShadowMap::depthShader = depthShader;
    }

    void setLightSpaceMatrix(const Mat4 &lightSpaceMatrix) {
        ShadowMap::lightSpaceMatrix = lightSpaceMatrix;
    }
    void bind(){
        glBindTexture(GL_TEXTURE_2D,depthTexture);
    }
    void showControlPanel() {
        ImGui::Begin("ShadowMapController");
        ImGui::SliderFloat("zNear",&(zNear),0.0f,1.0f);
        ImGui::SliderFloat("zFar",&zFar,5.0f,1000.0f);
        ImGui::SliderFloat("orthSize",&orthSize,10.0f,1000.0f);
        ImGui::End();
    }
    ~ShadowMap()=default;
};


#endif //RACINGCAR_SHADOWMAP_H
