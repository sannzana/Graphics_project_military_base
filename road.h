#ifndef MILITARY_ROAD_H
#define MILITARY_ROAD_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

class MilitaryRoad {
public:
    Cube& cube;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // road size
    float roadW = 18.0f;      // width (X)
    float roadL = 220.0f;     // length (Z)
    float roadT = 0.08f;      // thickness (increased from 0.03f)

    // line sizes
    float sideLineW = 0.18f;
    float sideLineInset = 0.70f;

    float dashW = 0.22f;
    float dashL = 2.0f;
    float dashGap = 1.6f;

    glm::vec3 roadColor = glm::vec3(0.10f, 0.10f, 0.10f);
    glm::vec3 yellowColor = glm::vec3(0.92f, 0.78f, 0.08f);
    glm::vec3 whiteColor = glm::vec3(0.92f, 0.92f, 0.92f);

    MilitaryRoad(Cube& cubeRef) : cube(cubeRef) {}

    void setPosition(const glm::vec3& p) { pos = p; }

    void draw(Shader& shader, const glm::mat4& parent = glm::mat4(1.0f)) {
        // main road body
        drawBox(shader, parent,
            pos + glm::vec3(0.0f, roadT * 0.5f, 0.0f),
            glm::vec3(roadW, roadT, roadL),
            roadColor);

        // left yellow line
        drawBox(shader, parent,
            pos + glm::vec3(-(roadW * 0.5f) + sideLineInset, roadT + 0.002f, 0.0f),
            glm::vec3(sideLineW, 0.01f, roadL * 0.98f),
            yellowColor);

        // right yellow line
        drawBox(shader, parent,
            pos + glm::vec3((roadW * 0.5f) - sideLineInset, roadT + 0.002f, 0.0f),
            glm::vec3(sideLineW, 0.01f, roadL * 0.98f),
            yellowColor);

        // white center dashes
        float startZ = -roadL * 0.5f + 4.0f;
        float endZ = roadL * 0.5f - 4.0f;
        float step = dashL + dashGap;

        for (float z = startZ; z < endZ; z += step) {
            drawBox(shader, parent,
                pos + glm::vec3(0.0f, roadT + 0.003f, z),
                glm::vec3(dashW, 0.01f, dashL),
                whiteColor);
        }
    }

private:
    glm::mat4 I = glm::mat4(1.0f);

    glm::mat4 boxM(const glm::mat4& P, const glm::vec3& center, const glm::vec3& size) const {
        // your cube is 0..1 based
        return P * glm::translate(I, center - size * 0.5f) * glm::scale(I, size);
    }

    void setCommon(Shader& shader, const glm::vec3& color, float shine = 8.0f) {
        shader.use();
        shader.setVec3("material.ambient", color * 0.35f);
        shader.setVec3("material.diffuse", color);
        shader.setVec3("material.specular", glm::vec3(0.08f));
        shader.setFloat("material.shininess", shine);
        shader.setVec3("objectColor", color);
        shader.setInt("platFloorMode", -1);
        shader.setBool("useTex", false);
    }

    void drawBox(Shader& shader, const glm::mat4& parent,
        const glm::vec3& center,
        const glm::vec3& size,
        const glm::vec3& color) {
        glm::mat4 model = boxM(parent, center, size);
        setCommon(shader, color);
        shader.setMat4("model", model);
        cube.drawCubeWithTexture(shader, model);
    }
};

#endif