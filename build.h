#pragma once
#ifndef BUILD_H
#define BUILD_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "shader.h"
#include "cube.h"

// ======================================================
// helper draw functions
// ======================================================
static void bld_solidCube(unsigned int vao, Shader& sh,
    const glm::mat4& m,
    float r, float g, float b,
    float shine = 24.f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * 0.28f, g * 0.28f, b * 0.28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(0.25f, 0.25f, 0.25f));
    sh.setFloat("material.shininess", shine);
    sh.setMat4("model", m);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

static void bld_texCube(Cube& cube, Shader& sh, const glm::mat4& m)
{
    cube.drawCubeWithTexture(sh, m);
}

class Build
{
public:
    Cube& wallCube;   // build.png
    Cube& floorCube;  // floor.png / concrete
    Cube& doorCube;   // wood.png

    // world placement holder
    glm::vec3 origin;

    // LOCAL building size (origin is NOT used in geometry)
    float bW;   // inner width
    float bD;   // inner depth
    float bH;   // wall height
    float bF;   // floor thickness

    // door
    float doorW = 2.4f;
    float doorH = 4.0f;
    float doorOpenAngle = 0.0f;
    float targetDoorOpenAngle = 0.0f;
    bool doorOpen = false;

    // windows
    float winW = 2.2f;
    float winH = 1.5f;
    float winBottom = 2.2f;

    float leftWinOpenAmount = 0.0f;
    float targetLeftWinOpen = 0.0f;
    bool leftWinOpen = false;

    float rightWinOpenAmount = 0.0f;
    float targetRightWinOpen = 0.0f;
    bool rightWinOpen = false;

    static constexpr float WT = 0.40f;   // wall thickness
    static constexpr float RT = 0.30f;   // roof thickness

    Build(Cube& wall, Cube& floor, Cube& door,
        glm::vec3 worldOrigin = glm::vec3(0.f),
        float width = 14.f,
        float depth = 18.f,
        float height = 6.5f,
        float raise = 0.15f)
        : wallCube(wall), floorCube(floor), doorCube(door),
        origin(worldOrigin),
        bW(width), bD(depth), bH(height), bF(raise)
    {
    }

    void update(float deltaTime)
    {
        float doorSpeed = 90.0f;
        float winSpeed = 1.0f;

        if (doorOpenAngle < targetDoorOpenAngle)
            doorOpenAngle = std::min(doorOpenAngle + doorSpeed * deltaTime, targetDoorOpenAngle);
        else if (doorOpenAngle > targetDoorOpenAngle)
            doorOpenAngle = std::max(doorOpenAngle - doorSpeed * deltaTime, targetDoorOpenAngle);

        if (leftWinOpenAmount < targetLeftWinOpen)
            leftWinOpenAmount = std::min(leftWinOpenAmount + winSpeed * deltaTime, targetLeftWinOpen);
        else if (leftWinOpenAmount > targetLeftWinOpen)
            leftWinOpenAmount = std::max(leftWinOpenAmount - winSpeed * deltaTime, targetLeftWinOpen);

        if (rightWinOpenAmount < targetRightWinOpen)
            rightWinOpenAmount = std::min(rightWinOpenAmount + winSpeed * deltaTime, targetRightWinOpen);
        else if (rightWinOpenAmount > targetRightWinOpen)
            rightWinOpenAmount = std::max(rightWinOpenAmount - winSpeed * deltaTime, targetRightWinOpen);
    }

    void toggleDoor()
    {
        doorOpen = !doorOpen;
        targetDoorOpenAngle = doorOpen ? 95.0f : 0.0f;
    }

    void toggleLeftWindow()
    {
        leftWinOpen = !leftWinOpen;
        targetLeftWinOpen = leftWinOpen ? 0.45f : 0.0f;
    }

    void toggleRightWindow()
    {
        rightWinOpen = !rightWinOpen;
        targetRightWinOpen = rightWinOpen ? 0.45f : 0.0f;
    }

    // ---------- LOCAL geometry extents ----------
    float outerW() const { return bW + WT * 2.f; }
    float outerD() const { return bD + WT * 2.f; }

    float leftX()   const { return -outerW() * 0.5f; }
    float rightX()  const { return  outerW() * 0.5f; }
    float backZ()   const { return -outerD() * 0.5f; }
    float frontZ()  const { return  outerD() * 0.5f; }

    float floorY()      const { return 0.0f; }
    float platformTop() const { return bF; }
    float wallBottom()  const { return bF; }
    float wallTop()     const { return bF + bH; }
    float roofBottom()  const { return wallTop(); }
    float roofTop()     const { return wallTop() + RT; }

    void draw(unsigned int cubeVAO, Shader& solidSh, Shader& texSh,
        const glm::mat4& modelMatrix = glm::mat4(1.0f)) const
    {
        drawFloor(cubeVAO, solidSh, texSh, modelMatrix);
        drawLeftWall(cubeVAO, solidSh, texSh, modelMatrix);
        drawRightWall(cubeVAO, solidSh, texSh, modelMatrix);
        drawBackWall(cubeVAO, solidSh, texSh, modelMatrix);
        drawFrontWallWithDoorAndWindows(cubeVAO, solidSh, texSh, modelMatrix);
        drawRoof(cubeVAO, solidSh, texSh, modelMatrix);
    }

private:
    const glm::mat4 I = glm::mat4(1.f);

    glm::mat4 Box(float minX, float minY, float minZ,
        float sx, float sy, float sz) const
    {
        return glm::scale(
            glm::translate(I, glm::vec3(minX, minY, minZ)),
            glm::vec3(sx, sy, sz)
        );
    }

    void drawFloor(unsigned int, Shader&, Shader& ts, const glm::mat4& M) const
    {
        bld_texCube(floorCube, ts,
            M * Box(leftX(), floorY(), backZ(), outerW(), bF, outerD()));
    }

    void drawLeftWall(unsigned int, Shader&, Shader& ts, const glm::mat4& M) const
    {
        bld_texCube(wallCube, ts,
            M * Box(leftX(), wallBottom(), backZ(), WT, bH, outerD()));
    }

    void drawRightWall(unsigned int, Shader&, Shader& ts, const glm::mat4& M) const
    {
        bld_texCube(wallCube, ts,
            M * Box(rightX() - WT, wallBottom(), backZ(), WT, bH, outerD()));
    }

    void drawBackWall(unsigned int, Shader&, Shader& ts, const glm::mat4& M) const
    {
        bld_texCube(wallCube, ts,
            M * Box(leftX(), wallBottom(), backZ(), outerW(), bH, WT));
    }

    void drawFrontWallWithDoorAndWindows(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& M) const
    {
        float z = frontZ() - WT;

        // centered door in LOCAL space
        float doorMinX = -doorW * 0.5f;
        float doorMaxX = doorW * 0.5f;

        // windows in LOCAL space
        float leftWinCenterX = -bW * 0.28f;
        float rightWinCenterX = bW * 0.28f;

        float leftWinMinX = leftWinCenterX - winW * 0.5f;
        float leftWinMaxX = leftWinCenterX + winW * 0.5f;
        float rightWinMinX = rightWinCenterX - winW * 0.5f;
        float rightWinMaxX = rightWinCenterX + winW * 0.5f;

        // ----- fixed wall pieces -----

        // far left strip
        bld_texCube(wallCube, ts,
            M * Box(leftX(), wallBottom(), z,
                leftWinMinX - leftX(), bH, WT));

        // between left window and door
        bld_texCube(wallCube, ts,
            M * Box(leftWinMaxX, wallBottom(), z,
                doorMinX - leftWinMaxX, bH, WT));

        // between door and right window
        bld_texCube(wallCube, ts,
            M * Box(doorMaxX, wallBottom(), z,
                rightWinMinX - doorMaxX, bH, WT));

        // far right strip
        bld_texCube(wallCube, ts,
            M * Box(rightWinMaxX, wallBottom(), z,
                rightX() - rightWinMaxX, bH, WT));

        // below left window
        bld_texCube(wallCube, ts,
            M * Box(leftWinMinX, wallBottom(), z,
                winW, winBottom, WT));

        // above left window
        bld_texCube(wallCube, ts,
            M * Box(leftWinMinX, wallBottom() + winBottom + winH, z,
                winW, wallTop() - (wallBottom() + winBottom + winH), WT));

        // below right window
        bld_texCube(wallCube, ts,
            M * Box(rightWinMinX, wallBottom(), z,
                winW, winBottom, WT));

        // above right window
        bld_texCube(wallCube, ts,
            M * Box(rightWinMinX, wallBottom() + winBottom + winH, z,
                winW, wallTop() - (wallBottom() + winBottom + winH), WT));

        // above door
        bld_texCube(wallCube, ts,
            M * Box(doorMinX, wallBottom() + doorH, z,
                doorW, wallTop() - (wallBottom() + doorH), WT));

        // ---- door frame ----
        float frameT = 0.08f;
        bld_solidCube(vao, ss,
            M * Box(doorMinX - frameT, wallBottom(), z, frameT, doorH, WT),
            0.20f, 0.20f, 0.20f, 22.f);

        bld_solidCube(vao, ss,
            M * Box(doorMaxX, wallBottom(), z, frameT, doorH, WT),
            0.20f, 0.20f, 0.20f, 22.f);

        bld_solidCube(vao, ss,
            M * Box(doorMinX - frameT, wallBottom() + doorH, z,
                doorW + 2.0f * frameT, frameT, WT),
            0.20f, 0.20f, 0.20f, 22.f);

        // ---- animated door ----
        drawDoor(vao, ss, ts, M, doorMinX, z);

        // ---- animated windows ----
        drawWindow(vao, ss, M, leftWinMinX, wallBottom() + winBottom, z, leftWinOpenAmount, true);
        drawWindow(vao, ss, M, rightWinMinX, wallBottom() + winBottom, z, rightWinOpenAmount, false);
    }

    void drawDoor(unsigned int vao, Shader& ss, Shader& ts,
        const glm::mat4& M, float doorMinX, float z) const
    {
        const float panelT = 0.05f;

        // hinge at left side of the door opening
        glm::mat4 doorM = M;
        doorM = doorM * glm::translate(I, glm::vec3(doorMinX, wallBottom(), z - 0.01f));
        doorM = doorM * glm::rotate(I, glm::radians(-doorOpenAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        doorM = doorM * glm::scale(I, glm::vec3(doorW, doorH, panelT));

        bld_texCube(doorCube, ts, doorM);

        // handle
        glm::mat4 handleM = M;
        handleM = handleM * glm::translate(I, glm::vec3(doorMinX, wallBottom(), z - 0.01f));
        handleM = handleM * glm::rotate(I, glm::radians(-doorOpenAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        handleM = handleM * glm::translate(I, glm::vec3(doorW - 0.18f, doorH * 0.50f, 0.0f));

        bld_solidCube(vao, ss,
            handleM * Box(0.0f, 0.0f, 0.0f, 0.05f, 0.12f, 0.03f),
            0.86f, 0.82f, 0.72f, 64.f);
    }

    void drawWindow(unsigned int vao, Shader& ss, const glm::mat4& M,
        float minX, float minY, float z,
        float openAmt, bool isLeft) const
    {
        // frame
        float f = 0.06f;
        bld_solidCube(vao, ss,
            M * Box(minX - f, minY - f, z, winW + 2.f * f, f, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);

        bld_solidCube(vao, ss,
            M * Box(minX - f, minY + winH, z, winW + 2.f * f, f, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);

        bld_solidCube(vao, ss,
            M * Box(minX - f, minY, z, f, winH, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);

        bld_solidCube(vao, ss,
            M * Box(minX + winW, minY, z, f, winH, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);

        // glass panels: slide inward along local Z
        float glassInsetZ = z - 0.015f - openAmt;

        // left half
        bld_solidCube(vao, ss,
            M * Box(minX, minY, glassInsetZ,
                winW * 0.5f - 0.03f, winH, 0.035f),
            0.35f, 0.60f, 0.78f, 80.f);

        // right half
        bld_solidCube(vao, ss,
            M * Box(minX + winW * 0.5f + 0.03f, minY, glassInsetZ,
                winW * 0.5f - 0.03f, winH, 0.035f),
            0.35f, 0.60f, 0.78f, 80.f);

        // center split frame
        bld_solidCube(vao, ss,
            M * Box(minX + winW * 0.5f - 0.03f, minY, z,
                0.06f, winH, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);

        // middle horizontal frame
        bld_solidCube(vao, ss,
            M * Box(minX, minY + winH * 0.5f - 0.03f, z,
                winW, 0.06f, 0.03f),
            0.16f, 0.16f, 0.16f, 18.f);
    }

    void drawRoof(unsigned int, Shader&, Shader& ts, const glm::mat4& M) const
    {
        bld_texCube(wallCube, ts,
            M * Box(leftX(), roofBottom(), backZ(), outerW(), RT, outerD()));
    }
};

#endif // BUILD_H