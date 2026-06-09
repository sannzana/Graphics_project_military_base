#pragma once
#ifndef GARAGE_PLATFORM_H
#define GARAGE_PLATFORM_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

static void gp_solidCube(unsigned int vao, Shader& sh,
    const glm::mat4& m,
    float r, float g, float b,
    float shine = 24.f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * .28f, g * .28f, b * .28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(.25f, .25f, .25f));
    sh.setFloat("material.shininess", shine);
    sh.setMat4("model", m);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

static void gp_texCube(Cube& cube, Shader& sh, const glm::mat4& m)
{
    cube.drawCubeWithTexture(sh, m);
}

class GaragePlatform
{
public:
    Cube& wallCube;
    Cube& floorCube;

    glm::vec3 origin;   // center of platform footprint on ground

    float gW;           // inner width
    float gD;           // inner depth
    float gH;           // wall height
    float gF;           // platform thickness/raise

    static constexpr float WT = 0.40f;   // wall thickness
    static constexpr float RT = 0.30f;   // roof thickness

    static constexpr float LPH = 7.20f;
    static constexpr float LPR = 0.12f;
    static constexpr float LHR = 0.42f;
    static constexpr float LHA = 1.10f;

    GaragePlatform(Cube& wall, Cube& floor,
        glm::vec3 worldOrigin = glm::vec3(0.f),
        float width = 14.f,
        float depth = 18.f,
        float height = 7.f,
        float raise = 1.2f)
        : wallCube(wall), floorCube(floor),
        origin(worldOrigin),
        gW(width), gD(depth), gH(height), gF(raise)
    {
    }

    float outerW() const { return gW + WT * 2.f; }
    float outerD() const { return gD + WT * 2.f; }

    float leftX()   const { return origin.x - outerW() * 0.5f; }
    float rightX()  const { return origin.x + outerW() * 0.5f; }
    float backZ()   const { return origin.z - outerD() * 0.5f; }
    float frontZ()  const { return origin.z + outerD() * 0.5f; }

    float floorY()      const { return 0.0f; }
    float platformTop() const { return gF; }
    float wallBottom()  const { return gF; }
    float wallTop()     const { return gF + gH; }
    float roofBottom()  const { return wallTop(); }
    float roofTop()     const { return wallTop() + RT; }

    glm::vec3 interiorLightPos() const
    {
        return glm::vec3(origin.x, gF + gH * 0.72f, origin.z);
    }

    glm::vec3 lampLightPosFrontLeft() const
    {
        return glm::vec3(leftX(), gF + LPH - 0.20f, frontZ());
    }

    glm::vec3 lampLightPosFrontRight() const
    {
        return glm::vec3(rightX(), gF + LPH - 0.20f, frontZ());
    }

    glm::vec3 lampLightPosBackLeft() const
    {
        return glm::vec3(leftX(), gF + LPH - 0.20f, backZ());
    }

    glm::vec3 lampLightPosBackRight() const
    {
        return glm::vec3(rightX(), gF + LPH - 0.20f, backZ());
    }

    void draw(unsigned int cubeVAO, Shader& solidSh, Shader& texSh) const
    {
        drawFloor(cubeVAO, solidSh, texSh);
        drawRamp(cubeVAO, solidSh, texSh);

        drawLeftWall(cubeVAO, solidSh, texSh);
        drawRightWall(cubeVAO, solidSh, texSh);
        drawBackWall(cubeVAO, solidSh, texSh);

        drawRoof(cubeVAO, solidSh, texSh);
        drawLamps(cubeVAO, solidSh);
    }

private:
    const glm::mat4 I = glm::mat4(1.f);

    // IMPORTANT:
    // cube vertices are 0..1, so translation must be to MIN CORNER, not center
    glm::mat4 Box(float minX, float minY, float minZ,
        float sx, float sy, float sz) const
    {
        return glm::scale(
            glm::translate(I, glm::vec3(minX, minY, minZ)),
            glm::vec3(sx, sy, sz)
        );
    }

    void drawFloor(unsigned int vao, Shader& ss, Shader& ts) const
    {
        gp_texCube(floorCube, ts,
            Box(leftX(), floorY(), backZ(), outerW(), gF, outerD()));
    }

    void drawRamp(unsigned int vao, Shader& ss, Shader& ts) const
    {
        float totalLen = gF * 3.0f;

        float laneGap = 0.0f;

        float leftInnerX = origin.x - gW * 0.5f;

        float rampW = gW * 0.75f - laneGap * 0.5f;
        float stairW = gW * 0.25f - laneGap * 0.5f;

        float rampX = leftInnerX;
        float stairX = leftInnerX + gW * 0.75f + laneGap * 0.5f;

        // =========================
        // 1) RAMP PART (3/4 WIDTH)
        // =========================
        float rampThickness = 0.22f;
        float angle = glm::degrees(atanf(gF / totalLen));
        float hyp = sqrtf(gF * gF + totalLen * totalLen);

        glm::mat4 pivot = glm::translate(
            I,
            glm::vec3(rampX, gF - rampThickness + 0.01f, frontZ() - 0.03f)
        );
        glm::mat4 rot = glm::rotate(I, glm::radians(angle), glm::vec3(1.f, 0.f, 0.f));
        glm::mat4 sc = glm::scale(I, glm::vec3(rampW, rampThickness, hyp));

        gp_texCube(floorCube, ts, pivot * rot * sc);

        // single support block under ramp front
        // this removes the fake stair look created by multiple fill segments
        float supportLen = totalLen * 0.35f;
        gp_solidCube(vao, ss,
            Box(rampX, 0.0f, frontZ() + totalLen - supportLen, rampW, gF * 0.30f, supportLen),
            0.30f, 0.28f, 0.26f);

        // =========================
        // 2) STAIR PART (1/4 WIDTH)
        // =========================
        int steps = 4;
        float stepLen = totalLen / steps;
        float stepH = gF / steps;

        for (int i = 0; i < steps; i++)
        {
            float h = gF - (i + 1) * stepH;
            if (h < 0.0f) h = 0.0f;

            float z = frontZ() + i * stepLen;

            gp_solidCube(vao, ss,
                Box(stairX, 0.0f, z, stairW, h, stepLen + 0.02f),
                0.30f, 0.28f, 0.26f);
        }
    }
    void drawLeftWall(unsigned int vao, Shader& ss, Shader& ts) const
    {
        gp_texCube(wallCube, ts,
            Box(leftX(), wallBottom(), backZ(), WT, gH, outerD()));
    }

    void drawRightWall(unsigned int vao, Shader& ss, Shader& ts) const
    {
        gp_texCube(wallCube, ts,
            Box(rightX() - WT, wallBottom(), backZ(), WT, gH, outerD()));
    }

    void drawBackWall(unsigned int vao, Shader& ss, Shader& ts) const
    {
        gp_texCube(wallCube, ts,
            Box(leftX(), wallBottom(), backZ(), outerW(), gH, WT));
    }

    void drawRoof(unsigned int vao, Shader& ss, Shader& ts) const
    {
        gp_texCube(wallCube, ts,
            Box(leftX(), roofBottom(), backZ(), outerW(), RT, outerD()));
    }

    void drawLamps(unsigned int vao, Shader& ss) const
    {
        drawOneLamp(vao, ss, glm::vec3(leftX(), gF, frontZ()), +1); // FL
        drawOneLamp(vao, ss, glm::vec3(rightX(), gF, frontZ()), -1); // FR
        drawOneLamp(vao, ss, glm::vec3(leftX(), gF, backZ()), +1); // BL
        drawOneLamp(vao, ss, glm::vec3(rightX(), gF, backZ()), -1); // BR
    }

    void drawOneLamp(unsigned int vao, Shader& ss, glm::vec3 base, int armDir) const
    {
        gp_solidCube(vao, ss,
            Box(base.x - 0.25f, base.y, base.z - 0.25f, 0.50f, 0.24f, 0.50f),
            0.30f, 0.30f, 0.32f);

        gp_solidCube(vao, ss,
            Box(base.x - LPR * 0.5f, base.y + 0.24f, base.z - LPR * 0.5f, LPR, LPH, LPR),
            0.50f, 0.50f, 0.52f);

        float armY = base.y + 0.24f + LPH;
        float armMinX = (armDir > 0) ? base.x : base.x - LHA;

        gp_solidCube(vao, ss,
            Box(armMinX, armY - LPR * 0.5f, base.z - LPR * 0.5f, LHA, LPR, LPR),
            0.50f, 0.50f, 0.52f);

        float hx = (armDir > 0) ? base.x + LHA : base.x - LHA;

        gp_solidCube(vao, ss,
            Box(hx - (LHR * 1.8f) * 0.5f, armY - 0.20f - (LHR * 0.60f) * 0.5f, base.z - (LHR * 1.8f) * 0.5f,
                LHR * 1.8f, LHR * 0.60f, LHR * 1.8f),
            0.96f, 0.92f, 0.72f, 64.f);

        gp_solidCube(vao, ss,
            Box(hx - (LHR * 2.1f) * 0.5f, armY - 0.02f - (LHR * 0.20f) * 0.5f, base.z - (LHR * 2.1f) * 0.5f,
                LHR * 2.1f, LHR * 0.20f, LHR * 2.1f),
            0.20f, 0.20f, 0.22f);

        gp_solidCube(vao, ss,
            Box(hx - (LHR * 1.5f) * 0.5f, armY - 0.34f - (LHR * 0.16f) * 0.5f, base.z - (LHR * 1.5f) * 0.5f,
                LHR * 1.5f, LHR * 0.16f, LHR * 1.5f),
            0.18f, 0.18f, 0.18f);
    }
};

#endif // GARAGE_PLATFORM_H