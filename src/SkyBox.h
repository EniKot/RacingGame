//
// Created by 29839 on 2023/4/24.
//

#ifndef RACINGCAR_SKYBOX_H
#define RACINGCAR_SKYBOX_H

#include "CubeTexture.h"
#include <iostream>
#include <cmath>

float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f};
class SkyBox {
private:
    unsigned int VAO;
    unsigned int VBO;
    Shared<CubeTexture> cubeMap;
    Shared<Shader> shader;
public:
    SkyBox(const std::vector<std::string> &texture_faces){
        cubeMap = createShared<CubeTexture>(texture_faces);
        shader = createShared<Shader>("../Shader/CubeMapVertex.glsl", "../Shader/CubeMapFragment.glsl");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        shader->bind();
        shader->setInt("cubeMap",0);
    }
    void DrawSkyBox(){
        glDepthFunc(GL_LEQUAL);
        shader->bind();
        glDepthMask(GL_FALSE);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        cubeMap->bind();
        glDrawArrays(GL_TRIANGLES,0,36);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

    }
    Shared<Shader> getShader(){
        return this->shader;
    }
    Shared<CubeTexture> getCubeTexture(){
        return this->cubeMap;
    }
    ~SkyBox(){}

};


#endif //RACINGCAR_SKYBOX_H
