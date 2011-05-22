
#ifndef BATCH_H
#define BATCH_H

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

#include <OpenGL/OpenGL.h>

#include "glm/glm.hpp" // glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "IO.h"
#include "Texture.h"

enum {
  VERTEX = 0,
  NORMAL = 2,
  TEXTURE = 3
};

class Batch {
  
  
  std::vector<float> vertices_;
  std::vector<float> normals_;
  std::vector<float> texels_;
  int indices;
  
  glm::vec3 scale_;
  glm::vec4 diffuse_;
  glm::vec4 specular_;
  glm::vec4 ambient_;
  
  GLuint vertexArrayObject;
  
  GLuint textureId_;
  
public:
  
  inline void setScale(const glm::vec3& scale) {
    scale_.x = scale.x;
    scale_.y = scale.y;
    scale_.z = scale.z;
  }
    
  inline void addVertex(float x, float y, float z) {
    vertices_.push_back(x);
    vertices_.push_back(y);
    vertices_.push_back(z);
    indices++;
  }
  
  inline void addNormal(float x, float y, float z) {
    normals_.push_back(x);
    normals_.push_back(y);
    normals_.push_back(z);    
  }
  
  inline void addTexel(float s, float t) {
    texels_.push_back(s);
    texels_.push_back(t);
  }
  
  inline void setDiffuse(float r, float g, float b, float a) {
    diffuse_.r = r;
    diffuse_.g = g;
    diffuse_.b = b;
    diffuse_.a = a;
  }
  
  inline void setAmbient(float r, float g, float b, float a) {
    ambient_.r = r;
    ambient_.g = g;
    ambient_.b = b;
    ambient_.a = a;
  }
  
  inline void setSpecular(float r, float g, float b, float a) { 
    specular_.r = r;
    specular_.g = g;
    specular_.b = b;
    specular_.a = a;    
  }
  
  inline void setTexture(const std::string& path) {
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    unsigned int width, height;
    
    BYTE* bits = Texture::loadImage(path, &width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
    glGenerateMipmap(GL_TEXTURE_2D);    
  }
  
  void finalize() {
    glGenVertexArraysAPPLE(1, &vertexArrayObject);
    glBindVertexArrayAPPLE(vertexArrayObject);
    
    GLuint vertexBufferObject;
    
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), &vertices_[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(VERTEX);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    GLuint normalBufferObject;
        
    glGenBuffers(1, &normalBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(float), &normals_[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    GLuint textureBufferObject;
    
    glGenBuffers(1, &textureBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
    glBufferData(GL_ARRAY_BUFFER, texels_.size() * sizeof(float), &texels_[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(TEXTURE);
    glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
    glVertexAttribPointer(TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, 0);
  }
  
  void render(GLuint shaderProg, const glm::mat4& modelViewMatrix, const glm::mat4& projectionMatrix) {
        
    glBindTexture(GL_TEXTURE_2D, textureId_);
        
    glm::mat4 scale = glm::scale(modelViewMatrix, scale_);
    glm::mat4 mv(scale);
    glm::mat4 mvp = projectionMatrix * mv;
    
//    glm::mat4 inverseCameraRotation = glm::inverse(rotation);
//    GLint uniformInverseCamera = glGetUniformLocation(shaderProg, "mInverseCamera");
//    glUniformMatrix4fv(uniformInverseCamera, 1, GL_FALSE, glm::value_ptr(inverseCameraRotation));
    
    GLint uniformColorMap = glGetUniformLocation(shaderProg, "colorMap");
    glUniform1i(uniformColorMap, 0);
    
    GLint uniformMVMatrix = glGetUniformLocation(shaderProg, "mvMatrix");
    glUniformMatrix4fv(uniformMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
    
    GLint  uniformLocation = glGetUniformLocation(shaderProg, "mvpMatrix");
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mv));
    GLint uniformNormalMatrix = glGetUniformLocation(shaderProg, "normalMatrix");
    glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        
    GLint  uniformColor = glGetUniformLocation(shaderProg, "diffuseColor");
    glUniform4fv(uniformColor, 1, glm::value_ptr(diffuse_));
    
    GLint uniformSpecColor = glGetUniformLocation(shaderProg, "specularColor");
    glUniform4fv(uniformSpecColor, 1, glm::value_ptr(specular_));
    
    GLint uniformAmbientColor = glGetUniformLocation(shaderProg, "ambientColor");
    glUniform4fv(uniformAmbientColor, 1, glm::value_ptr(ambient_));
    
    glm::vec4 lightPosition(100, 100, 100, 1.0f);
    GLint uniformLightPosition = glGetUniformLocation(shaderProg, "vLightPosition");
    glUniform3fv(uniformLightPosition, 1, glm::value_ptr(lightPosition));

    glBindVertexArrayAPPLE(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, indices);
    glDisableClientState(GL_VERTEX_ARRAY);    
  }
};

#endif