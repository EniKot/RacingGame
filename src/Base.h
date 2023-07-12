//
// Created by 29839 on 2023/4/29.
//

#ifndef RACINGGAME_BASE_H
#define RACINGGAME_BASE_H


#define DEFAULT_SCREEN_WEIGHT 800
#define DEFAULT_SCREEN_HEIGHT 600



#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>


template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Unique<T> createUnique(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}




template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Shared<T> createShared(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

using Vec4 = glm::vec4;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Mat4 = glm::mat4;
using Mat3 = glm::mat3;
/*
inline float EaseOutCurve(float x){
    return sqrt(1 - pow(x - 1, 2));
}
inline float EaseInCurve(float x){
    return 1.0 - sqrt(1 - pow(x, 2));
}
*/
#endif //RACINGGAME_BASE_H