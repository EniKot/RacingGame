#include "Car.h"

Car::Car(){}
void Car::speedUp() {
    if(speed < max_speed-0.015f){
        speed += 0.015f;
    }else{
        speed = max_speed;
    }
    Vec3 forward = transform.getForward();
    moveVec = speed*forward;
}
void Car::back(){
    if(speed > min_speed+0.015f){
        speed -= 0.015f;
    }else{
        speed = min_speed;
    }
    Vec3 forward = transform.getForward();
    moveVec = speed*forward;
}
void Car::turnLeft() {
    float beta =atan(0.5*tan(glm::radians(delta_f)));
    float omega = speed*sin(beta)*2.0/L;
    float R = speed/omega;
    float z = R*sin(omega);
    float x = R-R*cos(omega);
    Vec3 right = -x*transform.getRight();
    Vec3 forward = z*transform.getForward();
    moveVec = right+forward;
    rotVec = Vec3(0,-omega,0);
}
void Car::turnRight(){
    float beta =atan(0.5*tan(glm::radians(delta_f)));
    float omega = speed*sin(beta)*2.0/L;
    float R = speed/omega;
    float z = R*sin(omega);
    float x = R-R*cos(omega);
    Vec3 right = x*transform.getRight();
    Vec3 forward = z*transform.getForward();
    moveVec = right+forward;
    rotVec = Vec3(0,omega,0);
}
void Car::slowDown(){
    if(speed>0.0045){
        speed -= 0.0045f;
    }else if(speed <-0.0045){
        speed += 0.0045f;
    }else{
        speed = 0.0;
    }
    Vec3 forward = transform.getForward();
    moveVec = speed*forward;
}
void Car::carMove() {
    transform.move(moveVec);
    transform.rotate(rotVec);
    moveVec = Vec3(0,0,0);
    rotVec = Vec3(0,0,0);
}

float Car::getSpeed() const {
    return speed;
}
