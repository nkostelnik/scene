#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
class Camera {
  
  float rotX;
  float rotY;
  
  glm::vec4 forward_;
  glm::vec4 left_;
  glm::vec4 up_;
  glm::vec3 origin_;
  
  float width_;
  float height_;
  int label_;
  
public:
  
  Camera(float width, float height, unsigned int label) {
    forward_ = glm::vec4(0, 0, -1, 1.0f);
    left_ = glm::vec4(-1, 0, 0, 1.0f);
    up_ = glm::vec4(0, 1, 0, 1.0f); 
    origin_ = glm::vec3(0, -1, 0);
    width_ = width;
    height_ = height;
    
    rotX = 0;
    rotY = 0;
    label_ = label;
  };
  
  unsigned int label() {
    return label_;
  }  
  
  void update(float forwardVelocity, float leftVelocity, float upVelocity, float deltaTime) {
    origin_.x += (forward_.x * forwardVelocity) * deltaTime;
    origin_.y += (forward_.y * forwardVelocity) * deltaTime;
    origin_.z += (forward_.z * forwardVelocity) * deltaTime;
    
    origin_.x += (left_.x * leftVelocity) * deltaTime;
    origin_.y += (left_.y * leftVelocity) * deltaTime;
    origin_.z += (left_.z * leftVelocity) * deltaTime;
    
    origin_.x += (up_.x * upVelocity) * deltaTime;
    origin_.y += (up_.y * upVelocity) * deltaTime;
    origin_.z += (up_.z * upVelocity) * deltaTime;
  }
  
  glm::mat4 rotation() {
    glm::mat4 eyeRotationY = glm::rotate(glm::mat4(1.0f), rotY, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 eyeRotationX = glm::rotate(eyeRotationY, rotX, glm::vec3(0.0f, 1.0f, 0.0f));      
    return eyeRotationX;
  }
  
  glm::mat4 projection() {
    return glm::perspective(60.0f, width_ / height_, 0.5f, 200.0f);
  }
  
  glm::mat4 view() {
    glm::mat4 eyeTranslation = glm::translate(rotation(), origin_);
    return eyeTranslation;
  }
  
  glm::mat4 viewProjection() {
    return projection() * view();
  }
  
  glm::vec3 position() {
    return origin_;
  }
  
  void rotateX(float angle) {
    rotX = angle;
    glm::mat4 cameraRotated = glm::rotate(glm::mat4(1.0), -angle, glm::vec3(0.0f, 1.0f, 0.0f));
    forward_ = forward_ * cameraRotated;
    left_ = left_ * cameraRotated;
  }
  
  void rotateY(float angle) {
    rotY = angle;
  }
  
  void set_position(const glm::vec3& position) {
    origin_ = -position;
  }
  
};

#endif