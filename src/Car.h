//
// Created by 29839 on 2023/4/27.
//

#ifndef RACINGGAME_ENTITY_H
#define RACINGGAME_ENTITY_H


#include "Entity.h"

class Car : public Entity{
private:
    const float max_speed = 1.0f;
    const float min_speed = -0.5f;

    float speed = 0.0f;
public:
    float getSpeed() const;

private:
    float L = 2.0f;
    //内轮转角
    float delta_f = 80.0f;
    Vec3 moveVec = Vec3(0,0,0);
    Vec3 rotVec = Vec3(0,0,0);
public:
    Car();
    void turnLeft();
    void turnRight();
    void speedUp();
    void back();
    void slowDown();
    void carMove();

};


#endif //RACINGGAME_ENTITY_H
