//
// Created by 29839 on 2023/4/24.
//

#ifndef RACINGCAR_ENTITY_H
#define RACINGCAR_ENTITY_H

#include "Mesh.h"

class Transform{
protected:

    //Local space information
    Vec3 pos = { 0.0f, 0.0f, 0.0f };
    Vec3 eulerRot = { 0.0f, 0.0f, 0.0f }; //In degrees
    Vec3 scale = { 1.0f, 1.0f, 1.0f };

    //Global space informaiton concatenate in matrix
    Mat4 modelMatrix = Mat4(1.0f);

    //Dirty flag
    bool _isDirty = true;

    Mat4 getLocalModelMatrix()
    {
        const Mat4 transformX = glm::rotate(Mat4(1.0f), glm::radians(eulerRot.x), Vec3(1.0f, 0.0f, 0.0f));
        const Mat4 transformY = glm::rotate(Mat4(1.0f), glm::radians(eulerRot.y), Vec3(0.0f, 1.0f, 0.0f));
        const Mat4 transformZ = glm::rotate(Mat4(1.0f), glm::radians(eulerRot.z), Vec3(0.0f, 0.0f, 1.0f));

        // Y * X * Z
        const Mat4 roationMatrix = transformY * transformX * transformZ;

        // translation * rotation * scale (also know as TRS matrix)
        return glm::translate(Mat4(1.0f), pos) * roationMatrix * glm::scale(Mat4(1.0f), scale);
    }
public:
    void computeModelMatrix(){
        modelMatrix = getLocalModelMatrix();
    }

    void computeModelMatrix(const Mat4& parentGlobalModelMatrix){
        modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
    }

    void setLocalPosition(const Vec3& newPosition){
        pos = newPosition;
        _isDirty = true;
    }

    void setLocalRotation(const Vec3& newRotation){
        eulerRot = newRotation;
        _isDirty = true;
    }

    void setLocalScale(const Vec3& newScale){
        scale = newScale;
        _isDirty = true;
    }

    Vec3 getGlobalPosition() const{
        return modelMatrix[3];
    }

    const Vec3& getLocalPosition() const{
        return pos;
    }

    const Vec3& getLocalRotation() const{
        return eulerRot;
    }

    const Vec3& getLocalScale() const{
        return scale;
    }

    const Mat4& getModelMatrix() const{
        return modelMatrix;
    }

    Vec3 getRight() const{
        return modelMatrix[0];
    }

    Vec3 getUp() const{
        return modelMatrix[1];
    }

    Vec3 getBackward() const{
        return modelMatrix[2];
    }

    Vec3 getForward() const{
        return -modelMatrix[2];
    }

    Vec3 getGlobalScale() const{
        return { glm::length(getRight()), glm::length(getUp()), glm::length(getBackward()) };
    }



    bool isDirty() const{
        return _isDirty;
    }
    void move(Vec3 moveVec){
        this->pos+=moveVec;
    }
    void rotate(Vec3 rotVec){
        this->eulerRot+=rotVec;
    }
};

class Entity {
protected:
    Shared<Shader> shader;
    Shared<Mesh> mesh;
public:
    Transform transform;

    void setShader(Shared<Shader> shader){this->shader = shader;}
    const Shared<Shader> &getShader() const{return this->shader;}
    void setMesh(Shared<Mesh> mesh) {
        this->mesh = mesh;
    }

    const Shared<Mesh> & getMesh() const {
        return mesh;
    }
    void Draw(){
        if(this->mesh){
            shader->bind();
            transform.computeModelMatrix();
            shader->setMat4("model",transform.getModelMatrix());
            mesh->Draw(shader);
        }
    }
    void Draw(Shared<Shader> myShader){
        if(this->mesh){
            myShader->bind();
            transform.computeModelMatrix();
            myShader->setMat4("model",transform.getModelMatrix());
            mesh->Draw(myShader);
        }
    }
};


#endif //RACINGCAR_ENTITY_H
