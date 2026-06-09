#ifndef BARRACK_H
#define BARRACK_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

extern float barrackDoorAngle;
extern bool barrackDoorOpening;

void updateBarrackDoor();

void drawBarrack(
    unsigned int& cubeVAO,
    Shader& lightingShader,
    Shader& lightingShaderWithTexture,
    const glm::mat4& parentModel,
    Cube& woodCube,
    Cube& roofCube
);

#endif