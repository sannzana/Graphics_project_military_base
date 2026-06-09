#ifndef MILITARY_GARAGE_PLATFORM_H
#define MILITARY_GARAGE_PLATFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "cylinder.h"

class MilitaryGaragePlatform {
public:
    Cube& texCube;      // use Cube made with wall/floor textures
    Shape& cyl;

    // hook this to your main.cpp global/platform mode
    int* platFloorModePtr = nullptr;

    // placement
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

    // overall size
    float bodyW = 18.0f;
    float bodyH = 6.0f;
    float bodyL = 26.0f;

    float floorH = 2.6f;   // raised platform height
    float wallT = 0.35f;
    float roofT = 0.28f;

    float rampL = 8.0f;   // garage slope length
    float rampT = 0.35f;

    bool nightMode = false;

    MilitaryGaragePlatform(Cube& cubeRef, Shape& cylRef, int* floorModeAddress = nullptr)
        : texCube(cubeRef), cyl(cylRef), platFloorModePtr(floorModeAddress) {
    }

    void setPosition(const glm::vec3& p) { pos = p; }
    void setNightMode(bool mode) { nightMode = mode; }

    void draw(Shader& shader, const glm::mat4& parent = glm::mat4(1.0f)) {
        drawFloor(shader, parent);
        drawRamp(shader, parent);

        drawLeftWall(shader, parent);
        drawRightWall(shader, parent);
        drawBackWall(shader, parent);
        drawRoof(shader, parent);

        drawEntrySidePosts(shader, parent);
        drawSideSkirts(shader, parent);
        drawLamps(shader, parent);
    }

private:
    glm::mat4 I = glm::mat4(1.0f);

    glm::mat4 boxM(const glm::mat4& P, const glm::vec3& p, const glm::vec3& s) {
        return P * glm::translate(I, p) * glm::scale(I, s);
    }

    glm::mat4 cylM(const glm::mat4& P,
        const glm::vec3& p,
        const glm::vec3& s,
        float ang = 0.0f,
        const glm::vec3& axis = glm::vec3(1, 0, 0)) {
        glm::mat4 m = P * glm::translate(I, p);
        if (ang != 0.0f) m = m * glm::rotate(I, glm::radians(ang), axis);
        return m * glm::scale(I, s);
    }

    void setCommon(Shader& shader, const glm::vec3& color, float shine = 16.0f) {
        shader.use();
        shader.setVec3("material.ambient", color * 0.28f);
        shader.setVec3("material.diffuse", color);
        shader.setVec3("material.specular", glm::vec3(0.30f));
        shader.setFloat("material.shininess", shine);
        shader.setVec3("objectColor", color);
    }

    void setWallMode(Shader& shader) {
        shader.use();
        shader.setInt("platFloorMode", -1);   // normal textured object path
        shader.setBool("useTex", true);
    }

    void setFloorMode(Shader& shader) {
        shader.use();
        int mode = platFloorModePtr ? *platFloorModePtr : 1;
        shader.setInt("platFloorMode", mode); // 0=color, 1=texture, 2=blend
        shader.setBool("useTex", false);
    }

    void drawCubeTex(Shader& shader,
        const glm::mat4& model,
        const glm::vec3& color,
        bool asFloor = false) {
        setCommon(shader, color);
        shader.setMat4("model", model);

        if (asFloor) setFloorMode(shader);
        else setWallMode(shader);

        texCube.drawCubeWithTexture(shader, model);
    }

    void drawCylinderCol(Shader& shader,
        const glm::mat4& model,
        const glm::vec3& color,
        float shine = 24.0f) {
        setCommon(shader, color, shine);
        shader.setInt("platFloorMode", -1);
        shader.setBool("useTex", false);
        cyl.drawTexture(shader, 0, 0, 0, 0, 0, 0, 1, 1, 1, model);
    }

    void drawFloor(Shader& shader, const glm::mat4& parent) {
        // raised garage deck
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(0.0f, floorH * 0.5f, 0.0f),
                glm::vec3(bodyW, floorH, bodyL)),
            glm::vec3(0.76f, 0.74f, 0.70f),
            true);
    }

    void drawRamp(Shader& shader, const glm::mat4& parent) {
        // open side is FRONT (+Z)
        glm::mat4 m = parent;
        m = glm::translate(m, pos + glm::vec3(0.0f, floorH * 0.5f, bodyL * 0.5f + rampL * 0.42f));
        m = glm::rotate(m, glm::radians(-15.0f), glm::vec3(1, 0, 0));
        m = glm::scale(m, glm::vec3(bodyW - wallT * 2.0f, rampT, rampL));
        drawCubeTex(shader, m, glm::vec3(0.76f, 0.74f, 0.70f), true);

        // side ramp cheek walls
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(-(bodyW * 0.5f) + wallT * 0.5f, floorH * 0.38f, bodyL * 0.5f + rampL * 0.25f),
                glm::vec3(wallT, floorH * 0.75f, rampL * 0.70f)),
            glm::vec3(0.52f, 0.50f, 0.47f));

        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3((bodyW * 0.5f) - wallT * 0.5f, floorH * 0.38f, bodyL * 0.5f + rampL * 0.25f),
                glm::vec3(wallT, floorH * 0.75f, rampL * 0.70f)),
            glm::vec3(0.52f, 0.50f, 0.47f));
    }

    void drawLeftWall(Shader& shader, const glm::mat4& parent) {
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(-(bodyW * 0.5f) + wallT * 0.5f, floorH + bodyH * 0.5f, 0.0f),
                glm::vec3(wallT, bodyH, bodyL)),
            glm::vec3(0.70f, 0.70f, 0.70f));
    }

    void drawRightWall(Shader& shader, const glm::mat4& parent) {
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3((bodyW * 0.5f) - wallT * 0.5f, floorH + bodyH * 0.5f, 0.0f),
                glm::vec3(wallT, bodyH, bodyL)),
            glm::vec3(0.70f, 0.70f, 0.70f));
    }

    void drawBackWall(Shader& shader, const glm::mat4& parent) {
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(0.0f, floorH + bodyH * 0.5f, -(bodyL * 0.5f) + wallT * 0.5f),
                glm::vec3(bodyW, bodyH, wallT)),
            glm::vec3(0.70f, 0.70f, 0.70f));
    }

    void drawRoof(Shader& shader, const glm::mat4& parent) {
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(0.0f, floorH + bodyH + roofT * 0.5f, 0.0f),
                glm::vec3(bodyW, roofT, bodyL)),
            glm::vec3(0.32f, 0.38f, 0.44f));
    }

    void drawEntrySidePosts(Shader& shader, const glm::mat4& parent) {
        // front opening frame posts
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(-(bodyW * 0.5f) + wallT * 0.5f, floorH + bodyH * 0.5f, (bodyL * 0.5f) - wallT * 0.5f),
                glm::vec3(wallT, bodyH, wallT)),
            glm::vec3(0.68f, 0.68f, 0.68f));

        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3((bodyW * 0.5f) - wallT * 0.5f, floorH + bodyH * 0.5f, (bodyL * 0.5f) - wallT * 0.5f),
                glm::vec3(wallT, bodyH, wallT)),
            glm::vec3(0.68f, 0.68f, 0.68f));

        // top beam over entry
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(0.0f, floorH + bodyH - wallT * 0.5f, (bodyL * 0.5f) - wallT * 0.5f),
                glm::vec3(bodyW, wallT, wallT)),
            glm::vec3(0.68f, 0.68f, 0.68f));
    }

    void drawSideSkirts(Shader& shader, const glm::mat4& parent) {
        // low retaining side blocks for the raised platform
        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3(-(bodyW * 0.5f) - 0.20f, floorH * 0.5f, 0.0f),
                glm::vec3(0.40f, floorH, bodyL)),
            glm::vec3(0.16f, 0.16f, 0.17f));

        drawCubeTex(shader,
            boxM(parent,
                pos + glm::vec3((bodyW * 0.5f) + 0.20f, floorH * 0.5f, 0.0f),
                glm::vec3(0.40f, floorH, bodyL)),
            glm::vec3(0.16f, 0.16f, 0.17f));
    }

    void drawLamps(Shader& shader, const glm::mat4& parent) {
        glm::vec3 poleCol(0.58f, 0.62f, 0.68f);
        glm::vec3 glow = nightMode
            ? glm::vec3(1.0f, 0.92f, 0.60f)
            : glm::vec3(0.88f, 0.88f, 0.82f);

        // two lamps at entry side, one on left side and one on right side
        // not in front-center of the entry
        float lampY = floorH + 4.2f;
        float entryZ = pos.z + bodyL * 0.5f + 1.4f;

        // left-side lamp
        drawCylinderCol(shader,
            cylM(parent,
                glm::vec3(pos.x - bodyW * 0.55f, lampY, entryZ),
                glm::vec3(0.16f, 8.0f, 0.16f)),
            poleCol);

        drawCubeTex(shader,
            boxM(parent,
                glm::vec3(pos.x - bodyW * 0.55f + 0.75f, floorH + 8.0f, entryZ),
                glm::vec3(1.6f, 0.10f, 0.10f)),
            poleCol);

        drawCylinderCol(shader,
            cylM(parent,
                glm::vec3(pos.x - bodyW * 0.55f + 1.45f, floorH + 8.0f, entryZ),
                glm::vec3(0.22f, 0.44f, 0.22f)),
            glow, 64.0f);

        // right-side lamp
        drawCylinderCol(shader,
            cylM(parent,
                glm::vec3(pos.x + bodyW * 0.55f, lampY, entryZ),
                glm::vec3(0.16f, 8.0f, 0.16f)),
            poleCol);

        drawCubeTex(shader,
            boxM(parent,
                glm::vec3(pos.x + bodyW * 0.55f - 0.75f, floorH + 8.0f, entryZ),
                glm::vec3(1.6f, 0.10f, 0.10f)),
            poleCol);

        drawCylinderCol(shader,
            cylM(parent,
                glm::vec3(pos.x + bodyW * 0.55f - 1.45f, floorH + 8.0f, entryZ),
                glm::vec3(0.22f, 0.44f, 0.22f)),
            glow, 64.0f);
    }
};

#endif