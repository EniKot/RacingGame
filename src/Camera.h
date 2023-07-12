//
// Created by 29839 on 2023/4/24.
//

#ifndef RACINGGAME_CAMERA_H
#define RACINGGAME_CAMERA_H

#include "Base.h"

class Camera {
public:
    Camera(){
        init();
    }
    Camera(const Vec3& _pos,const Mat4& _projection)
            : pos(_pos),projection(_projection) {
        init();
    }

     ~Camera() = default;

    const Mat4& getProjection() const { return projection; }

    void init(){
        fov = 45.0f;
        front=glm::vec3(0.0,0.0,-1.0);
        up=Vec3(0.0,1.0,0.0);
        right=glm::normalize(glm::cross(front,up));
    }
    void setProjection(const Mat4 &projection) {
        Camera::projection = projection;
    }

    const Vec3 &getUp() const {
        return up;
    }

    void setUp(const Vec3 &up) {
        Camera::up = up;
    }

    const Vec3 &getRight() const {
        return right;
    }

    void setRight(const Vec3 &right) {
        Camera::right = right;
    }

    const Vec3 &getFront() const {
        return front;
    }

    void setFront(const Vec3 &front) {
        Camera::front = front;
    }

    const Vec3 &getPos() const {
        return pos;
    }

    void setPos(const Vec3 &pos) {
        Camera::pos = pos;
    }

    float getFov() const {
        return fov;
    }

    void setFov(float fov) {
        Camera::fov = fov;
    }
    void move(const Vec3& moveVec){
        this->pos += moveVec;
    }
    void rotate(const Vec3& rotVec){
        if(eulerRot.x+rotVec.x > 89.0f){
            eulerRot.x=89.0f;
        }
        else if(eulerRot.x+rotVec.x<-89.0f){
            eulerRot.x=-89.0f;
        }else {
            eulerRot.x+=rotVec.x;
        }
        eulerRot.y+=rotVec.y;
        eulerRot.z+=rotVec.z;
        front.x=cos(glm::radians(eulerRot.y))*cos(glm::radians(eulerRot.x));
        front.y=sin(glm::radians(eulerRot.x));
        front.z=cos(glm::radians(eulerRot.x))*sin(glm::radians(eulerRot.y));
        front=glm::normalize(front);
        right  = glm::normalize(glm::cross(front, up));
    }
    Mat4 getViewMatrix(){
        return glm::lookAt(this->pos,this->pos+front,this->up);
    }
protected:
    Mat4 projection = Mat4(1.0f);
    Vec3 up;
    Vec3 right;
    Vec3 front;
    Vec3 pos = {0.0f,0.0f,0.0f};
    Vec3 eulerRot = { 0.0f, 0.0f, 0.0f };
    float fov;
    float zFar,zNear;
};


#endif //RACINGGAME_CAMERA_H
