//
// Created by 29839 on 2023/4/24.
//

#ifndef RACINGCAR_SHADER_H
#define RACINGCAR_SHADER_H

#include <sstream>
#include <fstream>
#include "Texture.h"

class Shader {
public:
    GLuint id;

    Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexShaderPath);
            fShaderFile.open(fragmentShaderPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "Error:Failed to Read Shader File " << std::endl;
            std::cout << "VertexPath: " << vertexShaderPath << std::endl;
            std::cout << "FragmentPath: " << fragmentShaderPath << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        id = glCreateProgram();
        unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(vShader, 1, &vShaderCode, NULL);
        glShaderSource(fShader, 1, &fShaderCode, NULL);
        glCompileShader(vShader);
        checkCompileErrors(vShader, "VERTEX");

        glCompileShader(fShader);

        checkCompileErrors(fShader, "FRAGMENT");

        glAttachShader(id, vShader);
        glAttachShader(id, fShader);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }

    ~Shader() { glDeleteProgram(id); }

    void bind() {
        glUseProgram(id);
    }

    void unbind() {
        glUseProgram(0);
    }

    void setBool(const std::string &name, bool value) {

        glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) {
        bind();
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setIntArray(const std::string &name, int *values, uint32_t count) {
        glUniform1iv(glGetUniformLocation(id, name.c_str()), count, values);
    }

    void setFloat(const std::string &name, float value) {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat2(const std::string &name, const glm::vec2 &value) {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void setFloat2(const std::string &name, float x, float y) {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }

    void setFloat3(const std::string &name, const glm::vec3 &value) {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void setFloat3(const std::string &name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }

    void setFloat4(const std::string &name, const glm::vec4 &value) {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void setFloat4(const std::string &name, float x, float y, float z, float w) {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- -----------------------------------------------------" << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                          << "\n -- -----------------------------------------------------" << std::endl;
            }
        }
    }
};
#endif //RACINGCAR_SHADER_H
