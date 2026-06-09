#ifndef WATCHTOWER_H
#define WATCHTOWER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shader.h"
#include "cube.h"
#include "cylinder.h"

class WatchTower
{
private:
    Shape& cyl;
    Cube& woodCube;
    Cube& roofCube;

    glm::vec3 basePos;
    float scaleFactor;

public:
    WatchTower(Shape& cylinderShape, Cube& wood, Cube& roof,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        float scale = 1.0f)
        : cyl(cylinderShape), woodCube(wood), roofCube(roof),
        basePos(position), scaleFactor(scale) {
    }

    void setPosition(const glm::vec3& p) { basePos = p; }
    void setScale(float s) { scaleFactor = s; }

    void draw(Shader& shader)
    {
        glm::mat4 root = glm::mat4(1.0f);
        root = glm::translate(root, basePos);
        root = glm::scale(root, glm::vec3(scaleFactor));

        drawMainLegs(shader, root);
        drawCrossBraces(shader, root);
        drawMidFrame(shader, root);
        drawTopPlatform(shader, root);
        drawCabin(shader, root);
        drawRoof(shader, root);
        drawRoofSupports(shader, root);
        drawOuterRailings(shader, root);
        drawLadder(shader, root);
    }

private:
    void drawCylinderSegment(
        Shader& shader,
        const glm::mat4& parent,
        glm::vec3 pos,
        glm::vec3 rotDeg,
        glm::vec3 scaleXYZ)
    {
        glm::mat4 model = parent;
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(rotDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scaleXYZ);

        cyl.drawTexture(shader,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f,
            model);
    }

    void drawCubePart(
        Shader& shader,
        Cube& cubeObj,
        const glm::mat4& parent,
        glm::vec3 pos,
        glm::vec3 rotDeg,
        glm::vec3 scaleXYZ)
    {
        glm::mat4 model = parent;
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(rotDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scaleXYZ);

        cubeObj.drawCubeWithTexture(shader, model);
    }

    void drawStackedVerticalPole(
        Shader& shader,
        const glm::mat4& parent,
        glm::vec3 base,
        int count,
        float segmentHeight,
        float thickness)
    {
        for (int i = 0; i < count; i++)
        {
            drawCylinderSegment(
                shader,
                parent,
                glm::vec3(base.x, base.y + i * segmentHeight, base.z),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(thickness, segmentHeight, thickness)
            );
        }
    }

    void drawCylinderChain(
        Shader& shader,
        const glm::mat4& parent,
        glm::vec3 start,
        glm::vec3 step,
        int count,
        glm::vec3 rotDeg,
        glm::vec3 scaleXYZ)
    {
        for (int i = 0; i < count; i++)
        {
            drawCylinderSegment(
                shader,
                parent,
                start + step * (float)i,
                rotDeg,
                scaleXYZ
            );
        }
    }

    void drawMainLegs(Shader& shader, const glm::mat4& root)
    {
        float x1 = -4.5f, x2 = 4.5f;
        float z1 = -4.5f, z2 = 4.5f;

        drawStackedVerticalPole(shader, root, glm::vec3(x1, 0.0f, z1), 12, 1.25f, 0.22f);
        drawStackedVerticalPole(shader, root, glm::vec3(x2, 0.0f, z1), 12, 1.25f, 0.22f);
        drawStackedVerticalPole(shader, root, glm::vec3(x2, 0.0f, z2), 12, 1.25f, 0.22f);
        drawStackedVerticalPole(shader, root, glm::vec3(x1, 0.0f, z2), 12, 1.25f, 0.22f);
    }

    void drawCrossBraces(Shader& shader, const glm::mat4& root)
    {
        for (int level = 0; level < 3; level++)
        {
            float y = 1.5f + level * 3.6f;

            drawCylinderChain(shader, root,
                glm::vec3(-4.0f, y, 4.45f),
                glm::vec3(0.80f, 0.65f, 0.0f),
                10,
                glm::vec3(0.0f, 0.0f, -50.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(4.0f, y, 4.45f),
                glm::vec3(-0.80f, 0.65f, 0.0f),
                10,
                glm::vec3(0.0f, 0.0f, 50.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(-4.0f, y, -4.45f),
                glm::vec3(0.80f, 0.65f, 0.0f),
                10,
                glm::vec3(0.0f, 0.0f, -50.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(4.0f, y, -4.45f),
                glm::vec3(-0.80f, 0.65f, 0.0f),
                10,
                glm::vec3(0.0f, 0.0f, 50.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(-4.45f, y, -4.0f),
                glm::vec3(0.0f, 0.65f, 0.80f),
                10,
                glm::vec3(50.0f, 0.0f, 0.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(-4.45f, y, 4.0f),
                glm::vec3(0.0f, 0.65f, -0.80f),
                10,
                glm::vec3(-50.0f, 0.0f, 0.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(4.45f, y, -4.0f),
                glm::vec3(0.0f, 0.65f, 0.80f),
                10,
                glm::vec3(50.0f, 0.0f, 0.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));

            drawCylinderChain(shader, root,
                glm::vec3(4.45f, y, 4.0f),
                glm::vec3(0.0f, 0.65f, -0.80f),
                10,
                glm::vec3(-50.0f, 0.0f, 0.0f),
                glm::vec3(0.11f, 0.70f, 0.11f));
        }
    }

    void drawMidFrame(Shader& shader, const glm::mat4& root)
    {
        drawCylinderChain(shader, root, glm::vec3(-4.2f, 10.8f, -4.2f), glm::vec3(1.05f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.12f, 1.0f, 0.12f));
        drawCylinderChain(shader, root, glm::vec3(-4.2f, 10.8f, 4.2f), glm::vec3(1.05f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.12f, 1.0f, 0.12f));

        drawCylinderChain(shader, root, glm::vec3(-4.2f, 10.8f, -4.2f), glm::vec3(0.0f, 0.0f, 1.05f), 9,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.12f, 1.0f, 0.12f));
        drawCylinderChain(shader, root, glm::vec3(4.2f, 10.8f, -4.2f), glm::vec3(0.0f, 0.0f, 1.05f), 9,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.12f, 1.0f, 0.12f));
    }

    void drawTopPlatform(Shader& shader, const glm::mat4& root)
    {
        drawCubePart(shader, woodCube, root,
            glm::vec3(-5.2f, 11.0f, -5.2f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(10.4f, 0.35f, 10.4f));
    }

    void drawCabin(Shader& shader, const glm::mat4& root)
    {
        // floor cabin
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 11.35f, -3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.0f, 0.25f, 6.0f));

        // walls
        // front wall
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 11.60f, 2.75f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.0f, 2.8f, 0.25f));

        // back wall
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 11.60f, -3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.0f, 2.8f, 0.25f));

        // left wall
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 11.60f, -3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.25f, 2.8f, 6.0f));

        // right wall
        drawCubePart(shader, woodCube, root,
            glm::vec3(2.75f, 11.60f, -3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.25f, 2.8f, 6.0f));

        // small window look
        drawCubePart(shader, roofCube, root,
            glm::vec3(1.50f, 12.35f, 2.76f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.00f, 0.90f, 0.08f));
    }

    void drawRoof(Shader& shader, const glm::mat4& root)
    {
        // Cabin walls:
        // x = -3 to 3
        // z = -3 to 3
        // wall top roughly near y = 14.40

        // -------------------------
        // MAIN ROOF
        // -------------------------
        // Two halves must meet at center ridge.
        // Each half starts from wall top and rises to middle.

        // left roof half
        drawCubePart(shader, roofCube, root,
            glm::vec3(-3.05f, 14.28f, -3.15f),
            glm::vec3(0.0f, 0.0f, 26.0f),
            glm::vec3(3.25f, 0.18f, 6.30f));

        // right roof half
        drawCubePart(shader, roofCube, root,
            glm::vec3(-0.20f, 15.72f, -3.15f),
            glm::vec3(0.0f, 0.0f, -26.0f),
            glm::vec3(3.25f, 0.18f, 6.30f));

        // -------------------------
        // FRONT TRIANGLE FILLER
        // placed between front wall and roof
        // front wall face is around z = 2.75
        // -------------------------

        // bottom strip
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 14.30f, 2.78f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.0f, 0.22f, 0.12f));

        // level 2
        drawCubePart(shader, woodCube, root,
            glm::vec3(-2.35f, 14.62f, 2.79f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(4.7f, 0.22f, 0.12f));

        // level 3
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.70f, 14.94f, 2.80f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(3.4f, 0.22f, 0.12f));

        // level 4
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.05f, 15.24f, 2.81f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.1f, 0.20f, 0.12f));

        // top piece
        drawCubePart(shader, woodCube, root,
            glm::vec3(-0.45f, 15.50f, 2.82f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.9f, 0.18f, 0.12f));

        // -------------------------
        // BACK TRIANGLE FILLER
        // placed between back wall and roof
        // back wall face is around z = -3.0
        // -------------------------

        // bottom strip
        drawCubePart(shader, woodCube, root,
            glm::vec3(-3.0f, 14.30f, -3.00f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.0f, 0.22f, 0.12f));

        // level 2
        drawCubePart(shader, woodCube, root,
            glm::vec3(-2.35f, 14.62f, -3.01f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(4.7f, 0.22f, 0.12f));

        // level 3
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.70f, 14.94f, -3.02f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(3.4f, 0.22f, 0.12f));

        // level 4
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.05f, 15.24f, -3.03f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.1f, 0.20f, 0.12f));

        // top piece
        drawCubePart(shader, woodCube, root,
            glm::vec3(-0.45f, 15.50f, -3.04f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.9f, 0.18f, 0.12f));
    }

    void drawRoofSupports(Shader& shader, const glm::mat4& root)
    {
        drawStackedVerticalPole(shader, root, glm::vec3(-5.0f, 11.2f, -5.0f), 4, 1.1f, 0.12f);
        drawStackedVerticalPole(shader, root, glm::vec3(5.0f, 11.2f, -5.0f), 4, 1.1f, 0.12f);
        drawStackedVerticalPole(shader, root, glm::vec3(5.0f, 11.2f, 5.0f), 4, 1.1f, 0.12f);
        drawStackedVerticalPole(shader, root, glm::vec3(-5.0f, 11.2f, 5.0f), 4, 1.1f, 0.12f);
    }

    void drawOuterRailings(Shader& shader, const glm::mat4& root)
    {
        // front railing
        drawCylinderChain(shader, root, glm::vec3(-5.0f, 12.30f, 5.05f), glm::vec3(1.1f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.08f, 1.0f, 0.08f));
        drawCylinderChain(shader, root, glm::vec3(-5.0f, 11.75f, 5.05f), glm::vec3(1.1f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.08f, 1.0f, 0.08f));

        for (int i = 0; i < 9; i++)
        {
            drawStackedVerticalPole(shader, root, glm::vec3(-5.0f + i * 1.1f, 11.2f, 5.05f), 2, 0.40f, 0.06f);
        }

        // left railing
        drawCylinderChain(shader, root, glm::vec3(-5.05f, 12.30f, -5.0f), glm::vec3(0.0f, 0.0f, 1.1f), 10,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.08f, 1.0f, 0.08f));
        drawCylinderChain(shader, root, glm::vec3(-5.05f, 11.75f, -5.0f), glm::vec3(0.0f, 0.0f, 1.1f), 10,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.08f, 1.0f, 0.08f));

        for (int i = 0; i < 10; i++)
        {
            drawStackedVerticalPole(shader, root, glm::vec3(-5.05f, 11.2f, -5.0f + i * 1.1f), 2, 0.40f, 0.06f);
        }

        // right railing
        drawCylinderChain(shader, root, glm::vec3(5.05f, 12.30f, -5.0f), glm::vec3(0.0f, 0.0f, 1.1f), 10,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.08f, 1.0f, 0.08f));
        drawCylinderChain(shader, root, glm::vec3(5.05f, 11.75f, -5.0f), glm::vec3(0.0f, 0.0f, 1.1f), 10,
            glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.08f, 1.0f, 0.08f));

        for (int i = 0; i < 10; i++)
        {
            drawStackedVerticalPole(shader, root, glm::vec3(5.05f, 11.2f, -5.0f + i * 1.1f), 2, 0.40f, 0.06f);
        }

        // back railing
        drawCylinderChain(shader, root, glm::vec3(-5.0f, 12.30f, -5.05f), glm::vec3(1.1f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.08f, 1.0f, 0.08f));
        drawCylinderChain(shader, root, glm::vec3(-5.0f, 11.75f, -5.05f), glm::vec3(1.1f, 0.0f, 0.0f), 9,
            glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.08f, 1.0f, 0.08f));

        for (int i = 0; i < 9; i++)
        {
            drawStackedVerticalPole(shader, root, glm::vec3(-5.0f + i * 1.1f, 11.2f, -5.05f), 2, 0.40f, 0.06f);
        }
    }

    void drawLadder(Shader& shader, const glm::mat4& root)
    {
        // simple ladder attached to front side of platform
        // fully connected from ground to platform

        glm::vec3 leftRailBottom(-1.0f, 0.2f, 5.15f);
        glm::vec3 rightRailBottom(1.0f, 0.2f, 5.15f);

        // ladder side rails
        drawCylinderChain(shader, root,
            leftRailBottom,
            glm::vec3(0.0f, 0.75f, -0.02f),
            15,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.08f, 0.78f, 0.08f));

        drawCylinderChain(shader, root,
            rightRailBottom,
            glm::vec3(0.0f, 0.75f, -0.02f),
            15,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.08f, 0.78f, 0.08f));

        // top connection to platform
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.2f, 11.0f, 4.95f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.4f, 0.16f, 0.35f));

        // ladder steps
        for (int i = 0; i < 14; i++)
        {
            drawCubePart(shader, woodCube, root,
                glm::vec3(-0.9f, 0.85f + i * 0.75f, 5.02f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.8f, 0.10f, 0.18f));
        }

        // lower connection at ground
        drawCubePart(shader, woodCube, root,
            glm::vec3(-1.2f, 0.0f, 5.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.4f, 0.12f, 0.30f));
    }
};

#endif