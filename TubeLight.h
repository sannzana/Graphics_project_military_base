#ifndef TUBELIGHT_H
#define TUBELIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class TubeLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber;

    TubeLight(float posX, float posY, float posZ,
              float ambR, float ambG, float ambB,
              float diffR, float diffG, float diffB,
              float specR, float specG, float specB,
              float constant, float linear, float quadratic,
              int lightNum)
    {
        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = lightNum;
    }

    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();

        std::string b = "pointLights[" + std::to_string(lightNumber) + "]";

        lightingShader.setVec3(b + ".position", position);
        lightingShader.setVec3(b + ".ambient", ambientOn * ambient * 0.5f);  // 50% less bright
        lightingShader.setVec3(b + ".diffuse", diffuseOn * diffuse * 0.5f);  // 50% less bright
        lightingShader.setVec3(b + ".specular", specularOn * specular * 0.5f); // 50% less bright
        lightingShader.setFloat(b + ".k_c", k_c);
        lightingShader.setFloat(b + ".k_l", k_l * 0.7f);  // reduced linear attenuation (spreads better)
        lightingShader.setFloat(b + ".k_q", k_q * 0.5f);  // reduced quadratic attenuation (spreads better)
    }

    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }

    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }

private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif
