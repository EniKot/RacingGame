

#ifndef RACINGGAME_TEXTURE_H
#define RACINGGAME_TEXTURE_H

#include <stb_image.h>
#include "glad/glad.h"
#include "Base.h"
#include "GLFW/glfw3.h"

class Texture {

public:
    unsigned char* data;
    int width,height;
    unsigned int id;
    Texture(){};
    Texture(const char *path){
        int nrChannels;
        data=stbi_load(path,&width,&height,&nrChannels,0);
        glGenTextures(1,&id);
        glBindTexture(GL_TEXTURE_2D,id);
        unsigned int format;
        if(nrChannels==1){
            format=GL_RED;
        }else if(nrChannels==3){
            format=GL_RGB;
        }else if(nrChannels==4){
            format=GL_RGBA;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if(data){
            glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            std::cout<<"Failed To Load Texture:"<<path<<std::endl;
        }
        stbi_image_free(data);
    }
    void bind()const{
        glBindTexture(GL_TEXTURE_2D,id);
    }

    void unbind()const{
        glBindTexture(GL_TEXTURE_2D,0);
    }

    void destroy(){
        glDeleteTextures(1,&id);
    }
};


#endif //RACINGGAME_TEXTURE_H
