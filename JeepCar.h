#ifndef JEEPCAR_H
#define JEEPCAR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "cube.h"
#include "cylinder.h"

class JeepCar
{
private:
    Cube& bodyCube;      // jeep body texture
    Cube& darkCube;      // black parts
    Cube& glassCube;     // glass / light parts
    Shape& cyl;          // wheels, round lights, spare wheel

    glm::vec3 position;
    float scaleFactor;
    float rotationY;

    bool leftDoorOpen;
    bool rightDoorOpen;
    bool windshieldFolded;

public:
    JeepCar(Cube& bodyTexCube, Cube& darkTexCube, Cube& glassTexCube, Shape& cylinderShape,
        glm::vec3 pos = glm::vec3(0.0f),
        float scale = 1.0f,
        float rotY = 0.0f)
        : bodyCube(bodyTexCube),
        darkCube(darkTexCube),
        glassCube(glassTexCube),
        cyl(cylinderShape),
        position(pos),
        scaleFactor(scale),
        rotationY(rotY),
        leftDoorOpen(false),
        rightDoorOpen(false),
        windshieldFolded(false)
    {
    }

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setScale(float s) { scaleFactor = s; }
    void setRotationY(float r) { rotationY = r; }

    void setLeftDoorOpen(bool v) { leftDoorOpen = v; }
    void setRightDoorOpen(bool v) { rightDoorOpen = v; }
    void setWindshieldFolded(bool v) { windshieldFolded = v; }

    void toggleLeftDoor() { leftDoorOpen = !leftDoorOpen; }
    void toggleRightDoor() { rightDoorOpen = !rightDoorOpen; }
    void toggleWindshield() { windshieldFolded = !windshieldFolded; }

    void draw(Shader& shader)
    {
        glm::mat4 root = glm::mat4(1.0f);
        root = glm::translate(root, position);
        root = glm::rotate(root, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
        root = glm::scale(root, glm::vec3(scaleFactor));

        drawChassis(shader, root);
        drawBodyShell(shader, root);
        drawHood(shader, root);
        drawGrille(shader, root);
        drawFenders(shader, root);
        drawDoors(shader, root);
        drawWindshield(shader, root);
        drawRoofAndRear(shader, root);
        drawInterior(shader, root);
        drawBumpers(shader, root);
        drawWheels(shader, root);
        drawSpareWheel(shader, root);
        drawLights(shader, root);
        drawMirrors(shader, root);
    }

private:
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

    void drawCylinderPart(
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

    void drawWheelWithSpokes(Shader& shader, const glm::mat4& root, glm::vec3 center)
    {
        // outer tire: thick and heavy
        drawCylinderPart(shader, root,
            center,
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.72f, 0.38f, 0.72f));

        // inner rim
        drawCylinderPart(shader, root,
            center,
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.42f, 0.42f, 0.42f));

        // hub center
        drawCylinderPart(shader, root,
            center,
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.12f, 0.44f, 0.12f));

        // fake spokes using thin cubes
        for (int i = 0; i < 6; i++)
        {
            float ang = i * 30.0f;
            drawCubePart(shader, darkCube, root,
                center + glm::vec3(-0.02f, 0.0f, 0.0f),
                glm::vec3(0.0f, ang, 90.0f),
                glm::vec3(0.05f, 0.45f, 0.10f));
        }
    }

    void drawChassis(Shader& shader, const glm::mat4& root)
    {
        drawCubePart(shader, darkCube, root,
            glm::vec3(-3.9f, 0.55f, -1.05f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(6.4f, 0.22f, 2.1f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(-3.5f, 0.3f, -0.9f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.5f, 0.18f, 0.18f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(-3.5f, 0.3f, 0.72f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.5f, 0.18f, 0.18f));
    }

    void drawBodyShell(Shader& shader, const glm::mat4& root)
    {
        // lower side body
        drawCubePart(shader, bodyCube, root,
            glm::vec3(-3.7f, 0.8f, -1.15f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(5.8f, 0.95f, 2.3f));

        // cabin block
        drawCubePart(shader, bodyCube, root,
            glm::vec3(-2.0f, 1.75f, -1.15f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(3.35f, 1.35f, 2.3f));

        // rear box
        drawCubePart(shader, bodyCube, root,
            glm::vec3(0.95f, 1.7f, -1.15f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.4f, 1.4f, 2.3f));
    }

    void drawHood(Shader& shader, const glm::mat4& root)
    {
        drawCubePart(shader, bodyCube, root,
            glm::vec3(-5.0f, 1.0f, -1.1f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.7f, 0.75f, 2.2f));

        drawCubePart(shader, bodyCube, root,
            glm::vec3(-4.9f, 1.65f, -1.1f),
            glm::vec3(0.0f, 0.0f, -6.0f),
            glm::vec3(1.65f, 0.14f, 2.18f));
    }

    void drawGrille(Shader& shader, const glm::mat4& root)
    {
        drawCubePart(shader, darkCube, root,
            glm::vec3(-5.45f, 0.95f, -0.78f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.20f, 1.0f, 1.56f));

        for (int i = 0; i < 7; i++)
        {
            drawCubePart(shader, glassCube, root,
                glm::vec3(-5.47f, 1.05f, -0.62f + i * 0.22f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.04f, 0.60f, 0.08f));
        }
    }

    void drawFenders(Shader& shader, const glm::mat4& root)
    {
        // front fenders
        drawCubePart(shader, darkCube, root,
            glm::vec3(-4.55f, 0.95f, 0.95f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.2f, 0.25f, 0.35f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(-4.55f, 0.95f, -1.30f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.2f, 0.25f, 0.35f));

        // rear fenders
        drawCubePart(shader, darkCube, root,
            glm::vec3(0.2f, 0.95f, 0.95f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.35f, 0.25f, 0.35f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(0.2f, 0.95f, -1.30f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.35f, 0.25f, 0.35f));
    }

    void drawDoors(Shader& shader, const glm::mat4& root)
    {
        // LEFT DOOR
        {
            glm::mat4 doorRoot = root;
            doorRoot = glm::translate(doorRoot, glm::vec3(-2.1f, 1.35f, 1.18f)); // hinge side
            if (leftDoorOpen)
                doorRoot = glm::rotate(doorRoot, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            // outer door
            drawCubePart(shader, bodyCube, doorRoot,
                glm::vec3(0.0f, 0.0f, -0.06f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.6f, 1.15f, 0.12f));

            // door glass
            drawCubePart(shader, glassCube, doorRoot,
                glm::vec3(0.18f, 0.45f, -0.07f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.15f, 0.50f, 0.05f));

            // handle
            drawCubePart(shader, darkCube, doorRoot,
                glm::vec3(1.22f, 0.12f, -0.09f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.16f, 0.08f, 0.04f));
        }

        // RIGHT DOOR
        {
            glm::mat4 doorRoot = root;
            doorRoot = glm::translate(doorRoot, glm::vec3(-2.1f, 1.35f, -1.18f)); // hinge side
            if (rightDoorOpen)
                doorRoot = glm::rotate(doorRoot, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            drawCubePart(shader, bodyCube, doorRoot,
                glm::vec3(0.0f, 0.0f, -0.06f),
                glm::vec3(0.0f, 180.0f, 0.0f),
                glm::vec3(1.6f, 1.15f, 0.12f));

            drawCubePart(shader, glassCube, doorRoot,
                glm::vec3(0.18f, 0.45f, -0.02f),
                glm::vec3(0.0f, 180.0f, 0.0f),
                glm::vec3(1.15f, 0.50f, 0.05f));

            drawCubePart(shader, darkCube, doorRoot,
                glm::vec3(1.22f, 0.12f, 0.00f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.16f, 0.08f, 0.04f));
        }
    }

    void drawWindshield(Shader& shader, const glm::mat4& root)
    {
        glm::mat4 wsRoot = root;
        wsRoot = glm::translate(wsRoot, glm::vec3(-3.35f, 1.7f, 0.0f));

        if (windshieldFolded)
            wsRoot = glm::rotate(wsRoot, glm::radians(55.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        else
            wsRoot = glm::rotate(wsRoot, glm::radians(-18.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        // frame
        drawCubePart(shader, darkCube, wsRoot,
            glm::vec3(0.0f, 0.0f, -1.08f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.16f, 1.45f, 0.10f));

        drawCubePart(shader, darkCube, wsRoot,
            glm::vec3(0.0f, 0.0f, 0.98f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.16f, 1.45f, 0.10f));

        drawCubePart(shader, darkCube, wsRoot,
            glm::vec3(0.0f, 1.28f, -1.08f),
            glm::vec3(0.0f, 0.0f, 90.0f),
            glm::vec3(0.08f, 1.05f, 0.10f));

        drawCubePart(shader, darkCube, wsRoot,
            glm::vec3(0.0f, 1.28f, 0.98f),
            glm::vec3(0.0f, 0.0f, 90.0f),
            glm::vec3(0.08f, 1.05f, 0.10f));

        drawCubePart(shader, darkCube, wsRoot,
            glm::vec3(0.0f, 1.28f, -0.05f),
            glm::vec3(0.0f, 0.0f, 90.0f),
            glm::vec3(0.06f, 1.00f, 0.08f));

        // glass panes
        drawCubePart(shader, glassCube, wsRoot,
            glm::vec3(0.03f, 0.68f, -0.82f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.03f, 1.0f, 0.72f));

        drawCubePart(shader, glassCube, wsRoot,
            glm::vec3(0.03f, 0.68f, 0.08f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.03f, 1.0f, 0.72f));
    }

    void drawRoofAndRear(Shader& shader, const glm::mat4& root)
    {
        // hardtop roof
        drawCubePart(shader, darkCube, root,
            glm::vec3(-2.1f, 3.0f, -1.08f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(3.9f, 0.18f, 2.16f));

        // rear side windows
        drawCubePart(shader, glassCube, root,
            glm::vec3(0.55f, 2.18f, 1.02f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.95f, 0.62f, 0.06f));

        drawCubePart(shader, glassCube, root,
            glm::vec3(0.55f, 2.18f, -1.08f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.95f, 0.62f, 0.06f));

        // rear window
        drawCubePart(shader, glassCube, root,
            glm::vec3(2.17f, 2.08f, -0.82f),
            glm::vec3(0.0f, 90.0f, 0.0f),
            glm::vec3(0.95f, 0.72f, 0.05f));
    }

    void drawInterior(Shader& shader, const glm::mat4& root)
    {
        // front seats
        drawCubePart(shader, darkCube, root,
            glm::vec3(-1.55f, 1.05f, -0.72f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.55f, 0.38f, 0.62f));
        drawCubePart(shader, darkCube, root,
            glm::vec3(-1.65f, 1.35f, -0.72f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.16f, 0.65f, 0.62f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(-1.55f, 1.05f, 0.10f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.55f, 0.38f, 0.62f));
        drawCubePart(shader, darkCube, root,
            glm::vec3(-1.65f, 1.35f, 0.10f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.16f, 0.65f, 0.62f));

        // rear bench
        drawCubePart(shader, darkCube, root,
            glm::vec3(0.45f, 0.95f, -0.82f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.95f, 0.34f, 1.64f));
        drawCubePart(shader, darkCube, root,
            glm::vec3(0.22f, 1.24f, -0.82f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.14f, 0.54f, 1.64f));

        // steering
        drawCylinderPart(shader, root,
            glm::vec3(-2.2f, 1.55f, 0.55f),
            glm::vec3(65.0f, 0.0f, 0.0f),
            glm::vec3(0.18f, 0.05f, 0.18f));
    }

    void drawBumpers(Shader& shader, const glm::mat4& root)
    {
        drawCubePart(shader, darkCube, root,
            glm::vec3(-5.85f, 0.48f, -0.98f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.40f, 0.25f, 1.96f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(2.38f, 0.52f, -0.95f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.32f, 0.22f, 1.90f));
    }

    void drawWheels(Shader& shader, const glm::mat4& root)
    {
        drawWheelWithSpokes(shader, root, glm::vec3(-4.25f, 0.42f, 1.32f));
        drawWheelWithSpokes(shader, root, glm::vec3(-4.25f, 0.42f, -1.32f));
        drawWheelWithSpokes(shader, root, glm::vec3(0.55f, 0.42f, 1.32f));
        drawWheelWithSpokes(shader, root, glm::vec3(0.55f, 0.42f, -1.32f));
    }

    void drawSpareWheel(Shader& shader, const glm::mat4& root)
    {
        drawWheelWithSpokes(shader, root, glm::vec3(2.45f, 1.45f, 0.0f));
    }

    void drawLights(Shader& shader, const glm::mat4& root)
    {
        // front heavy round lights
        drawCylinderPart(shader, root,
            glm::vec3(-5.52f, 1.2f, -0.98f),
            glm::vec3(0.0f, 90.0f, 0.0f),
            glm::vec3(0.20f, 0.14f, 0.20f));

        drawCylinderPart(shader, root,
            glm::vec3(-5.52f, 1.2f, 0.98f),
            glm::vec3(0.0f, 90.0f, 0.0f),
            glm::vec3(0.20f, 0.14f, 0.20f));

        // rear lights
        drawCubePart(shader, glassCube, root,
            glm::vec3(2.30f, 0.95f, -1.08f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.18f, 0.14f));

        drawCubePart(shader, glassCube, root,
            glm::vec3(2.30f, 0.95f, 0.94f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.06f, 0.18f, 0.14f));
    }

    void drawMirrors(Shader& shader, const glm::mat4& root)
    {
        drawCylinderPart(shader, root,
            glm::vec3(-3.1f, 1.95f, 1.25f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.05f, 0.16f, 0.05f));

        drawCubePart(shader, darkCube, root,
            glm::vec3(-3.3f, 2.08f, 1.17f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.16f, 0.15f, 0.08f));
    }
};

#endif