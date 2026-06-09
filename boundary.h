#ifndef MILITARY_BOUNDARY_H
#define MILITARY_BOUNDARY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

class MilitaryBoundary {
public:
    Cube& wallCube;
    Cube& gateCube;
    int* platFloorModePtr = nullptr;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

    float boundaryW = 160.0f;
    float boundaryL = 150.0f;

    float wallH = 14.0f;
    float wallT = 0.85f;

    float gateOpening = 26.0f;
    float gateH = 10.5f;
    float gateT = 0.40f;
    float gateAngle = 0.0f;
    float maxGateAngle = 95.0f;
    bool gateOpenTarget = false;

    glm::vec3 wallColor = glm::vec3(0.72f, 0.72f, 0.72f);
    glm::vec3 gateColor = glm::vec3(0.90f, 0.90f, 0.90f);
    glm::vec3 barColor = glm::vec3(0.22f, 0.22f, 0.22f);

    MilitaryBoundary(Cube& wallCubeRef, Cube& gateCubeRef, int* floorModeAddress = nullptr)
        : wallCube(wallCubeRef), gateCube(gateCubeRef), platFloorModePtr(floorModeAddress) {
    }

    void setPosition(const glm::vec3& p) { pos = p; }
    void setGateOpen(bool open) { gateOpenTarget = open; }
    void toggleGate() { gateOpenTarget = !gateOpenTarget; }

    void update(float dt) {
        float target = gateOpenTarget ? maxGateAngle : 0.0f;
        float speed = 85.0f;

        if (gateAngle < target) {
            gateAngle += speed * dt;
            if (gateAngle > target) gateAngle = target;
        }
        if (gateAngle > target) {
            gateAngle -= speed * dt;
            if (gateAngle < target) gateAngle = target;
        }
    }

    void draw(Shader& shader, const glm::mat4& parent = glm::mat4(1.0f)) {
        const float halfW = boundaryW * 0.5f;
        const float halfL = boundaryL * 0.5f;
        const float wallHalfH = wallH * 0.5f;

        // left wall
        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(-halfW + wallT * 0.5f, wallHalfH, 0.0f),
                glm::vec3(wallT, wallH, boundaryL)),
            wallColor);

        // right wall
        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(halfW - wallT * 0.5f, wallHalfH, 0.0f),
                glm::vec3(wallT, wallH, boundaryL)),
            wallColor);

        // back wall
        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(0.0f, wallHalfH, -halfL + wallT * 0.5f),
                glm::vec3(boundaryW - 2.0f * wallT, wallH, wallT)),
            wallColor);

        const float frontUsableW = boundaryW - 2.0f * wallT;
        float clearGateOpening = gateOpening;

        if (clearGateOpening > frontUsableW - 2.0f) clearGateOpening = frontUsableW - 2.0f;
        if (clearGateOpening < 8.0f) clearGateOpening = 8.0f;

        const float frontSidePartW = (frontUsableW - clearGateOpening) * 0.5f;
        const float frontZ = halfL - wallT * 0.5f;

        // front-left wall
        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(-(clearGateOpening * 0.5f + frontSidePartW * 0.5f),
                    wallHalfH,
                    frontZ),
                glm::vec3(frontSidePartW, wallH, wallT)),
            wallColor);

        // front-right wall
        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3((clearGateOpening * 0.5f + frontSidePartW * 0.5f),
                    wallHalfH,
                    frontZ),
                glm::vec3(frontSidePartW, wallH, wallT)),
            wallColor);

        drawGateFrame(shader, parent, clearGateOpening, frontZ);
        drawGate(shader, parent, clearGateOpening, frontZ);
    }

private:
    glm::mat4 I = glm::mat4(1.0f);

    glm::mat4 boxM(const glm::mat4& P, const glm::vec3& p, const glm::vec3& s) const {
        // for your 0..1 cube geometry
        return P * glm::translate(I, p - s * 0.5f) * glm::scale(I, s);
    }

    void setCommon(Shader& shader, const glm::vec3& color, float shine = 24.0f) {
        shader.use();
        shader.setVec3("material.ambient", color * 0.30f);
        shader.setVec3("material.diffuse", color);
        shader.setVec3("material.specular", glm::vec3(0.28f));
        shader.setFloat("material.shininess", shine);
        shader.setVec3("objectColor", color);
    }

    void setWallMode(Shader& shader) {
        shader.use();
        shader.setInt("platFloorMode", -1);
        shader.setBool("useTex", true);
    }

    void drawCubeTex(Cube& cubeRef, Shader& shader, const glm::mat4& model, const glm::vec3& color) {
        setCommon(shader, color);
        shader.setMat4("model", model);
        setWallMode(shader);
        cubeRef.drawCubeWithTexture(shader, model);
    }

    void drawGateFrame(Shader& shader, const glm::mat4& parent, float clearGateOpening, float frontZ) {
        const float pillarW = 0.9f;
        const float pillarHalfH = gateH * 0.5f;

        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(-clearGateOpening * 0.5f - pillarW * 0.5f,
                    pillarHalfH,
                    frontZ),
                glm::vec3(pillarW, gateH, wallT)),
            wallColor);

        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(clearGateOpening * 0.5f + pillarW * 0.5f,
                    pillarHalfH,
                    frontZ),
                glm::vec3(pillarW, gateH, wallT)),
            wallColor);

        drawCubeTex(wallCube, shader,
            boxM(parent,
                pos + glm::vec3(0.0f,
                    gateH + 0.55f,
                    frontZ),
                glm::vec3(clearGateOpening + 2.0f * pillarW, 1.1f, wallT)),
            wallColor);
    }

    void drawGate(Shader& shader, const glm::mat4& parent, float clearGateOpening, float frontZ) {
        const float panelW = clearGateOpening * 0.5f;
        const float gateZ = frontZ - 0.02f;

        // small sink so the gate touches ground visually
        const float gateBottomY = pos.y - 0.02f;

        // LEFT PANEL
        // hinge at left side, opens outward
        {
            glm::mat4 m = parent;

            // hinge point at ground-bottom-left-front of the left gate panel
            m = glm::translate(m, glm::vec3(
                pos.x - clearGateOpening * 0.5f,
                gateBottomY,
                gateZ
            ));

            // rotate outward
            m = glm::rotate(m, glm::radians(-gateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            // because cube is 0..1 based, scale directly from hinge corner
            m = glm::scale(m, glm::vec3(panelW, gateH, gateT));

            drawCubeTex(gateCube, shader, m, gateColor);
        }

        // RIGHT PANEL
        // hinge at right side, opens outward
        {
            glm::mat4 m = parent;

            // move to right hinge
            m = glm::translate(m, glm::vec3(
                pos.x + clearGateOpening * 0.5f,
                gateBottomY,
                gateZ
            ));

            // rotate outward
            m = glm::rotate(m, glm::radians(gateAngle), glm::vec3(0.0f, 1.0f, 0.0f));

            // shift cube so hinge is on its right edge
            m = glm::translate(m, glm::vec3(-panelW, 0.0f, 0.0f));

            // scale panel
            m = glm::scale(m, glm::vec3(panelW, gateH, gateT));

            drawCubeTex(gateCube, shader, m, gateColor);
        }
    }
};

#endif