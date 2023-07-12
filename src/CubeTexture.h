//
// Created by 29839 on 2023/4/24.
//

#ifndef RACINGCAR_CUBETEXTURE_H
#define RACINGCAR_CUBETEXTURE_H

#include "Shader.h"
#include "stb_image.h"
class CubeTexture {

public:
    unsigned int cubeMapTextureID;
    CubeTexture(const std::vector<std::string> &texture_faces){
        int nrChannels;
        unsigned char* data;
        int width,height;
        glGenTextures(1,&cubeMapTextureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMapTextureID);
        unsigned int channel;
        for(unsigned int i=0;i<texture_faces.size();i++){
            data= stbi_load(texture_faces[i].c_str(),&width,&height,&nrChannels,0);
            channel=nrChannels==3?GL_RGB:GL_RGBA;
            if(data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, channel, width, height, 0, channel,
                             GL_UNSIGNED_BYTE, data);
            }else{
                std::cout<<"CubeMap Texture load Failed!,PATH:"<<texture_faces[i]<<std::endl;
            }
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    void bind(){
        glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMapTextureID);
    }
    void unbind(){
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    }

    ~CubeTexture(){
        glDeleteTextures(1,&cubeMapTextureID);
    }
};


#endif //RACINGCAR_CUBETEXTURE_H
