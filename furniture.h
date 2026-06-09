#pragma once
#ifndef FURNITURE_H
#define FURNITURE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"

// ======================================================
// helper: cube vertices are 0..1 based, so use min corner
// ======================================================
static glm::mat4 FBox(const glm::mat4& parent,
    float x, float y, float z,
    float sx, float sy, float sz)
{
    glm::mat4 m = glm::translate(parent, glm::vec3(x, y, z));
    m = glm::scale(m, glm::vec3(sx, sy, sz));
    return m;
}

static void furnSolidCube(unsigned int vao, Shader& sh,
    const glm::mat4& m,
    float r, float g, float b,
    float shine = 32.0f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * 0.35f, g * 0.35f, b * 0.35f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(0.35f, 0.35f, 0.35f));
    sh.setFloat("material.shininess", shine);
    sh.setMat4("model", m);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

// ======================================================
// BED
// vao, sh -> solid brown color for frame/structure
// ======================================================
static void drawBed(unsigned int vao, Shader& sh,
    Cube& woodCube,
    const glm::mat4& parent)
{
    // overall dimensions
    const float bedW = 3.2f;
    const float bedL = 6.2f;
    const float frameH = 0.22f;
    const float legH = 1.0f;
    const float railT = 0.18f;

    // Brown color for wooden frame parts
    const float brownR = 0.55f;
    const float brownG = 0.35f;
    const float brownB = 0.15f;

    // ---------- WOODEN FRAME (BROWN) ----------
    // side rails
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, legH, -bedL * 0.5f, railT, frameH, bedL),
        brownR, brownG, brownB, 32.0f);

    furnSolidCube(vao, sh,
        FBox(parent, bedW * 0.5f - railT, legH, -bedL * 0.5f, railT, frameH, bedL),
        brownR, brownG, brownB, 32.0f);

    // front rail
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, legH, bedL * 0.5f - railT, bedW, frameH, railT),
        brownR, brownG, brownB, 32.0f);

    // back rail
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, legH, -bedL * 0.5f, bedW, frameH, railT),
        brownR, brownG, brownB, 32.0f);

    // support board under mattress
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.10f, legH + 0.05f, -bedL * 0.5f + 0.10f,
            bedW - 0.20f, 0.10f, bedL - 0.20f),
        brownR, brownG, brownB, 32.0f);

    // legs
    const float legT = 0.22f;
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, 0.0f, -bedL * 0.5f, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, bedW * 0.5f - legT, 0.0f, -bedL * 0.5f, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, 0.0f, bedL * 0.5f - legT, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, bedW * 0.5f - legT, 0.0f, bedL * 0.5f - legT, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);

    // headboard posts
    const float postH = 2.4f;
    const float postT = 0.22f;
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f, legH, -bedL * 0.5f, postT, postH, postT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, bedW * 0.5f - postT, legH, -bedL * 0.5f, postT, postH, postT),
        brownR, brownG, brownB, 32.0f);

    // headboard top panel
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.20f, legH + 1.45f, -bedL * 0.5f + 0.02f,
            bedW - 0.40f, 0.35f, 0.14f),
        brownR, brownG, brownB, 32.0f);

    // headboard mid panel
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.28f, legH + 0.95f, -bedL * 0.5f + 0.04f,
            bedW - 0.56f, 0.22f, 0.10f),
        brownR, brownG, brownB, 32.0f);

    // ---------- MATTRESS (WHITE) ----------
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.18f, legH + 0.18f, -bedL * 0.5f + 0.18f,
            bedW - 0.36f, 0.55f, bedL - 0.36f),
        0.96f, 0.96f, 0.96f, 48.0f);

    // bed sheet layer
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.22f, legH + 0.72f, -bedL * 0.5f + 0.22f,
            bedW - 0.44f, 0.10f, bedL - 0.44f),
        0.93f, 0.93f, 0.93f, 28.0f);

    // pillows
    furnSolidCube(vao, sh,
        FBox(parent, -bedW * 0.5f + 0.38f, legH + 0.78f, -bedL * 0.5f + 0.32f,
            1.15f, 0.28f, 0.95f),
        0.98f, 0.98f, 0.98f, 64.0f);

    furnSolidCube(vao, sh,
        FBox(parent, 0.02f, legH + 0.78f, -bedL * 0.5f + 0.32f,
            1.15f, 0.28f, 0.95f),
        0.98f, 0.98f, 0.98f, 64.0f);
}

// ======================================================
// MEDICAL BOX
// boxCube -> box.png (white medical box texture)
// ======================================================
static void drawMedicalBox(unsigned int vao, Shader& sh,
    Cube& boxCube,
    const glm::mat4& parent)
{
    // base
    boxCube.drawCubeWithTexture(sh,
        FBox(parent, -0.9f, 0.0f, -0.7f, 1.8f, 1.15f, 1.4f));

    // lid
    boxCube.drawCubeWithTexture(sh,
        FBox(parent, -0.92f, 1.10f, -0.72f, 1.84f, 0.24f, 1.44f));

    // small top lip
    boxCube.drawCubeWithTexture(sh,
        FBox(parent, -0.95f, 1.02f, -0.75f, 1.90f, 0.08f, 1.50f));

    // side handle tabs
    furnSolidCube(vao, sh,
        FBox(parent, -1.02f, 0.45f, -0.18f, 0.08f, 0.35f, 0.36f),
        0.80f, 0.80f, 0.80f, 40.0f);

    furnSolidCube(vao, sh,
        FBox(parent, 0.94f, 0.45f, -0.18f, 0.08f, 0.35f, 0.36f),
        0.80f, 0.80f, 0.80f, 40.0f);
}

// ======================================================
// TABLE
// vao, sh -> solid brown color for frame/structure
// ======================================================
static void drawTable(unsigned int vao, Shader& sh,
    Cube& woodCube,
    const glm::mat4& parent)
{
    const float topW = 4.2f;
    const float topD = 2.0f;
    const float topH = 0.22f;
    const float legH = 1.55f;
    const float legT = 0.20f;

    // Brown color for wooden table parts
    const float brownR = 0.55f;
    const float brownG = 0.35f;
    const float brownB = 0.15f;

    // tabletop
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, legH, -topD * 0.5f, topW, topH, topD),
        brownR, brownG, brownB, 32.0f);

    // apron under top
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, legH - 0.18f, -topD * 0.5f, topW, 0.16f, 0.14f),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, legH - 0.18f, topD * 0.5f - 0.14f, topW, 0.16f, 0.14f),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, legH - 0.18f, -topD * 0.5f, 0.14f, 0.16f, topD),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, topW * 0.5f - 0.14f, legH - 0.18f, -topD * 0.5f, 0.14f, 0.16f, topD),
        brownR, brownG, brownB, 32.0f);

    // legs
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, 0.0f, -topD * 0.5f, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, topW * 0.5f - legT, 0.0f, -topD * 0.5f, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f, 0.0f, topD * 0.5f - legT, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);
    furnSolidCube(vao, sh,
        FBox(parent, topW * 0.5f - legT, 0.0f, topD * 0.5f - legT, legT, legH, legT),
        brownR, brownG, brownB, 32.0f);

    // lower shelf
    furnSolidCube(vao, sh,
        FBox(parent, -topW * 0.5f + 0.18f, 0.42f, -topD * 0.5f + 0.18f,
            topW - 0.36f, 0.12f, topD - 0.36f),
        brownR, brownG, brownB, 32.0f);
}

#endif