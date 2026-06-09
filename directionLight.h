//
//  directionLight.h
//  test
//

#ifndef directionLight_h
#define directionLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class DirectionLight {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionLight(
        float dirX, float dirY, float dirZ,
        float ambR, float ambG, float ambB,
        float diffR, float diffG, float diffB,
        float specR, float specG, float specB)
    {
        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
    }

    void setUpDirectionLight(Shader& lightingShader)
    {
        lightingShader.use();
        lightingShader.setVec3("directionLight.direction", direction);
        lightingShader.setVec3("directionLight.ambient", ambientOn * ambient);
        lightingShader.setVec3("directionLight.diffuse", diffuseOn * diffuse);
        lightingShader.setVec3("directionLight.specular", specularOn * specular);
    }

    void turnOff()
    {
        ambientOn = 0.0f;
        diffuseOn = 0.0f;
        specularOn = 0.0f;
    }

    void turnOn()
    {
        ambientOn = 1.0f;
        diffuseOn = 1.0f;
        specularOn = 1.0f;
    }

    void turnAmbientOn() { ambientOn = 1.0f; }
    void turnAmbientOff() { ambientOn = 0.0f; }
    void turnDiffuseOn() { diffuseOn = 1.0f; }
    void turnDiffuseOff() { diffuseOn = 0.0f; }
    void turnSpecularOn() { specularOn = 1.0f; }
    void turnSpecularOff() { specularOn = 0.0f; }

private:
    float ambientOn = 1.0f;
    float diffuseOn = 1.0f;
    float specularOn = 1.0f;
};

#endif /* directionLight_h */
#pragma once
