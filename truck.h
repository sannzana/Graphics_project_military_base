#ifndef TRUCK_H
#define TRUCK_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "cylinder.h"
#include <cmath>

namespace TRC {
    constexpr float BODY_R = 0.43f, BODY_G = 0.46f, BODY_B = 0.33f;
    constexpr float BODY2_R = 0.36f, BODY2_G = 0.39f, BODY2_B = 0.28f;
    constexpr float DARK_R = 0.10f, DARK_G = 0.10f, DARK_B = 0.10f;
    constexpr float METAL_R = 0.34f, METAL_G = 0.36f, METAL_B = 0.34f;
    constexpr float GLASS_R = 0.42f, GLASS_G = 0.42f, GLASS_B = 0.45f;
    constexpr float RUBBER_R = 0.08f, RUBBER_G = 0.08f, RUBBER_B = 0.09f;
    constexpr float HUB_R = 0.50f, HUB_G = 0.50f, HUB_B = 0.48f;
    constexpr float LIGHT_R = 0.92f, LIGHT_G = 0.89f, LIGHT_B = 0.74f;
}

enum class TruckRenderMode {
    ColorOnly = 0,
    TextureOnly = 1,
    Blended = 2
};

static void _tb(unsigned int vao, Shader& sh, const glm::mat4& m,
    float r, float g, float b, float sh2 = 16.f, float spec = 0.25f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * .28f, g * .28f, b * .28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(spec, spec, spec));
    sh.setFloat("material.shininess", sh2);
    sh.setMat4("model", m);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

static void _tt(Cube& c, Shader& sh, const glm::mat4& m)
{
    c.drawCubeWithTexture(sh, m);
}

static void _tc(Shape& cy, Shader& sh, const glm::mat4& P,
    glm::vec3 pos, float rx, float ry, float rz,
    float r, float g, float b, float sh2 = 24.f,
    const glm::mat4& rot = glm::mat4(1.f), float spec = 0.25f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * .28f, g * .28f, b * .28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(spec, spec, spec));
    sh.setFloat("material.shininess", sh2);

    glm::mat4 I(1.f);
    glm::mat4 m = P * glm::translate(I, pos) * rot * glm::scale(I, glm::vec3(rx, ry, rz));

    cy.drawColor(sh, 0, 0, 0, 0, 0, 0, 1, 1, 1, m, glm::vec3(r, g, b));
}

static void _tty(Shape& cy, Shader& sh, const glm::mat4& P,
    glm::vec3 pos, float rx, float ry, float rz,
    const glm::mat4& rot = glm::mat4(1.f))
{
    glm::mat4 I(1.f);
    glm::mat4 m = P * glm::translate(I, pos) * rot * glm::scale(I, glm::vec3(rx, ry, rz));
    cy.drawTexture(sh, 0, 0, 0, 0, 0, 0, 1, 1, 1, m);
}

class Truck {
public:
    Cube& bodyTexCube;     // should use truck.png
    Shape& tyreCyl;        // tyre texture / dark tyre cylinder
    Shape& cargoCyl;       // should use back.png

    TruckRenderMode renderMode = TruckRenderMode::TextureOnly;

    Truck(Cube& bodyRef, Shape& tyreRef, Shape& cargoRef)
        : bodyTexCube(bodyRef), tyreCyl(tyreRef), cargoCyl(cargoRef) {
    }

    void cycleRenderMode()
    {
        if (renderMode == TruckRenderMode::ColorOnly) renderMode = TruckRenderMode::TextureOnly;
        else if (renderMode == TruckRenderMode::TextureOnly) renderMode = TruckRenderMode::Blended;
        else renderMode = TruckRenderMode::ColorOnly;
    }

    void draw(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P, float wheelSpinDeg = 0.0f)
    {
        drawChassis(vao, ss, P);
        drawCab(vao, ss, ts, P);
        drawFront(vao, ss, ts, P);
        drawWindows(vao, ss, P);
        drawMirrors(vao, ss, P);
        drawCargoBed(vao, ss, ts, P);
        drawCargoCover(vao, ss, ts, P);
        drawBumperAndLights(vao, ss, P);

        drawWheel(vao, ss, P, { -AXLE_X, WR,  FRONT_Z }, wheelSpinDeg);
        drawWheel(vao, ss, P, { AXLE_X, WR,  FRONT_Z }, wheelSpinDeg);
        drawWheel(vao, ss, P, { -AXLE_X, WR,  MID_Z }, wheelSpinDeg);
        drawWheel(vao, ss, P, { AXLE_X, WR,  MID_Z }, wheelSpinDeg);
        drawWheel(vao, ss, P, { -AXLE_X, WR,  REAR_Z }, wheelSpinDeg);
        drawWheel(vao, ss, P, { AXLE_X, WR,  REAR_Z }, wheelSpinDeg);
    }

private:
    glm::mat4 I = glm::mat4(1.f);

    static constexpr float CAB_W = 2.45f;
    static constexpr float CAB_H = 1.95f;
    static constexpr float CAB_L = 2.05f;

    static constexpr float BED_W = 2.70f;
    static constexpr float BED_L = 5.30f;

    static constexpr float WR = 0.72f;   // thicker than before
    static constexpr float WW = 0.42f;   // much thicker tyre width

    static constexpr float AXLE_X = 1.42f;
    static constexpr float FRONT_Z = 2.28f;
    static constexpr float MID_Z = -0.20f;
    static constexpr float REAR_Z = -2.18f;

    glm::mat4 bx(const glm::mat4& P,
        float cx, float cy, float cz,
        float sx, float sy, float sz)
    {
        return P
            * glm::translate(I, { cx - sx * .5f, cy - sy * .5f, cz - sz * .5f })
            * glm::scale(I, { sx, sy, sz });
    }

    glm::mat4 bm(const glm::mat4& P,
        float x, float y, float z,
        float sx, float sy, float sz)
    {
        return P
            * glm::translate(I, { x, y, z })
            * glm::scale(I, { sx, sy, sz });
    }

    void drawBodyPiece(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& m,
        float r, float g, float b, float shininess = 18.f)
    {
        if (renderMode == TruckRenderMode::ColorOnly) {
            _tb(vao, ss, m, r, g, b, shininess, 0.20f);
        }
        else if (renderMode == TruckRenderMode::TextureOnly) {
            _tt(bodyTexCube, ts, m);
        }
        else {
            // Blended mode: 50% texture + 50% color
            // First draw texture at 50% opacity via lower material values
            _tt(bodyTexCube, ts, m);
            // Then blend color on top with mid-range specular for balanced effect
            _tb(vao, ss, m, r * 0.5f, g * 0.5f, b * 0.5f, shininess * 0.75f, 0.15f);
        }
    }

    void drawCargoPiece(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& m,
        float r, float g, float b, float shininess = 8.f)
    {
        if (renderMode == TruckRenderMode::ColorOnly) {
            _tb(vao, ss, m, r, g, b, shininess, 0.03f);
        }
        else if (renderMode == TruckRenderMode::TextureOnly) {
            cargoCyl.drawTexture(ts, 0, 0, 0, 0, 0, 0, 1, 1, 1, m);
        }
        else {
            // Blended mode: 50% texture + 50% color
            cargoCyl.drawTexture(ts, 0, 0, 0, 0, 0, 0, 1, 1, 1, m);
            _tb(vao, ss, m, r * 0.5f, g * 0.5f, b * 0.5f, shininess * 0.75f, 0.015f);
        }
    }

    void drawChassis(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        _tb(vao, ss, bx(P, 0.0f, 0.36f, -0.15f, 2.15f, 0.14f, 7.35f),
            TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 12.f, 0.12f);

        _tb(vao, ss, bx(P, 0.0f, 0.56f, FRONT_Z, 2.00f, 0.10f, 0.16f),
            TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 8.f, 0.08f);
        _tb(vao, ss, bx(P, 0.0f, 0.56f, MID_Z, 2.00f, 0.10f, 0.16f),
            TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 8.f, 0.08f);
        _tb(vao, ss, bx(P, 0.0f, 0.56f, REAR_Z, 2.00f, 0.10f, 0.16f),
            TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 8.f, 0.08f);
    }

    void drawCab(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float cabY = 1.02f;

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, cabY + 0.70f, 2.20f, CAB_W, 1.40f, CAB_L),
            TRC::BODY_R, TRC::BODY_G, TRC::BODY_B, 20.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, cabY + 1.45f, 1.92f, CAB_W, 0.44f, 1.12f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 18.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, cabY + 0.72f, 3.00f, 2.18f, 0.76f, 0.66f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 18.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, cabY + 1.18f, 2.88f, 2.04f, 0.30f, 0.50f),
            TRC::BODY_R, TRC::BODY_G, TRC::BODY_B, 18.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, 2.36f, 2.08f, 2.48f, 0.08f, 1.76f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);
    }

    void drawFront(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float zf = 3.38f;

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, 1.86f, zf, 2.12f, 0.96f, 0.08f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 18.f);

        _tb(vao, ss, bx(P, 0.0f, 1.78f, zf + 0.03f, 1.62f, 0.56f, 0.03f),
            0.05f, 0.05f, 0.05f, 8.f, 0.05f);

        _tb(vao, ss, bx(P, 0.0f, 1.02f, zf + 0.03f, 1.72f, 0.26f, 0.04f),
            0.04f, 0.04f, 0.04f, 8.f, 0.05f);

        for (int i = 0; i < 5; ++i) {
            float y = 1.55f + i * 0.10f;
            _tb(vao, ss, bx(P, 0.0f, y, zf + 0.05f, 1.50f, 0.020f, 0.020f),
                TRC::METAL_R, TRC::METAL_G, TRC::METAL_B, 12.f, 0.15f);
        }
    }

    void drawWindows(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        glm::mat4 leftW = P
            * glm::translate(I, glm::vec3(-1.00f, 1.82f, 3.02f))
            * glm::rotate(I, glm::radians(-21.0f), glm::vec3(1, 0, 0))
            * glm::scale(I, glm::vec3(0.96f, 0.66f, 0.045f));

        glm::mat4 rightW = P
            * glm::translate(I, glm::vec3(0.04f, 1.82f, 3.02f))
            * glm::rotate(I, glm::radians(-21.0f), glm::vec3(1, 0, 0))
            * glm::scale(I, glm::vec3(0.96f, 0.66f, 0.045f));

        _tb(vao, ss, leftW, TRC::GLASS_R, TRC::GLASS_G, TRC::GLASS_B, 40.f, 0.60f);
        _tb(vao, ss, rightW, TRC::GLASS_R, TRC::GLASS_G, TRC::GLASS_B, 40.f, 0.60f);

        _tb(vao, ss, bx(P, 0.0f, 1.82f, 3.06f, 0.05f, 0.78f, 0.04f),
            TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 12.f, 0.10f);

        _tb(vao, ss, bx(P, -1.24f, 1.64f, 2.22f, 0.04f, 0.62f, 0.78f),
            TRC::GLASS_R, TRC::GLASS_G, TRC::GLASS_B, 30.f, 0.50f);
        _tb(vao, ss, bx(P, 1.24f, 1.64f, 2.22f, 0.04f, 0.62f, 0.78f),
            TRC::GLASS_R, TRC::GLASS_G, TRC::GLASS_B, 30.f, 0.50f);

        _tb(vao, ss, bx(P, -0.42f, 1.47f, 3.14f, 0.60f, 0.025f, 0.025f),
            0.05f, 0.05f, 0.05f, 8.f, 0.05f);
        _tb(vao, ss, bx(P, 0.42f, 1.47f, 3.14f, 0.60f, 0.025f, 0.025f),
            0.05f, 0.05f, 0.05f, 8.f, 0.05f);
    }

    void drawMirrors(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        for (int s : { -1, 1 }) {
            float x = s * 1.48f;

            _tb(vao, ss, bx(P, x, 1.56f, 2.82f, 0.04f, 0.68f, 0.04f),
                TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 10.f, 0.10f);

            _tb(vao, ss, bx(P, x - s * 0.16f, 1.88f, 2.90f, 0.24f, 0.28f, 0.05f),
                TRC::DARK_R, TRC::DARK_G, TRC::DARK_B, 12.f, 0.10f);
        }
    }

    void drawCargoBed(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float bedY = 1.04f;
        float bedZ = -1.58f;

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, bedY, bedZ, BED_W, 0.16f, BED_L),
            TRC::BODY_R, TRC::BODY_G, TRC::BODY_B, 18.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, -BED_W * .5f - 0.03f, bedY + 0.36f, bedZ, 0.06f, 0.72f, BED_L),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);
        drawBodyPiece(vao, ss, ts,
            bx(P, BED_W * .5f + 0.03f, bedY + 0.36f, bedZ, 0.06f, 0.72f, BED_L),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);

        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, bedY + 0.36f, 1.05f, BED_W, 0.72f, 0.06f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);
        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, bedY + 0.36f, -4.20f, BED_W, 0.72f, 0.06f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);
    }

    void drawCargoCover(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        // cargo bed center
        float zc = -1.58f;

        // rotate cylinder so length goes along Z
        glm::mat4 rot = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));

        // IMPORTANT:
        // old one was too small and too high
        // new one:
        // - wider  -> fills side-to-side better
        // - longer -> fills front-to-back better
        // - lower  -> sits into the bed like a proper half-cylinder cover
        glm::vec3 coverPos = glm::vec3(0.0f, 1.70f, zc);
        glm::vec3 coverScale = glm::vec3(1.48f, 4.8f, 1.18f);

        glm::mat4 m = P
            * glm::translate(I, coverPos)
            * rot
            * glm::scale(I, coverScale);

        if (renderMode == TruckRenderMode::ColorOnly) {
            _tc(cargoCyl, ss, P, coverPos,
                coverScale.x, coverScale.y, coverScale.z,
                0.66f, 0.67f, 0.60f, 7.f, rot, 0.02f);
        }
        else if (renderMode == TruckRenderMode::TextureOnly) {
            cargoCyl.drawTexture(ts, 0, 0, 0, 0, 0, 0, 1, 1, 1, m);
        }
        else {
            cargoCyl.drawTexture(ts, 0, 0, 0, 0, 0, 0, 1, 1, 1, m);
            _tc(cargoCyl, ss, P, coverPos,
                coverScale.x, coverScale.y, coverScale.z,
                0.66f, 0.67f, 0.60f, 7.f, rot, 0.02f);
        }

        // front flap: make it match the wider cover
        _tb(vao, ss, bx(P, 0.0f, 1.88f, 1.02f, 2.52f, 0.86f, 0.05f),
            0.62f, 0.62f, 0.58f, 6.f, 0.02f);

        // rear flap: make it match the wider cover
        _tb(vao, ss, bx(P, 0.0f, 1.88f, -4.18f, 2.52f, 0.86f, 0.05f),
            0.62f, 0.62f, 0.58f, 6.f, 0.02f);

        // left hide panel
        drawBodyPiece(vao, ss, ts,
            bx(P, -1.17f, 1.38f, zc, 0.10f, 0.82f, 5.02f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);

        // right hide panel
        drawBodyPiece(vao, ss, ts,
            bx(P, 1.17f, 1.38f, zc, 0.10f, 0.82f, 5.02f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);

        // front hide panel
        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, 1.38f, 1.06f, 2.34f, 0.82f, 0.08f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);

        // rear hide panel
        drawBodyPiece(vao, ss, ts,
            bx(P, 0.0f, 1.38f, -4.22f, 2.34f, 0.82f, 0.08f),
            TRC::BODY2_R, TRC::BODY2_G, TRC::BODY2_B, 16.f);
    }

    void drawBumperAndLights(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float zf = 3.44f;

        _tb(vao, ss, bx(P, 0.0f, 0.74f, zf + 0.02f, 2.46f, 0.20f, 0.22f),
            0.08f, 0.08f, 0.08f, 16.f, 0.12f);

        _tb(vao, ss, bx(P, -0.86f, 0.92f, zf + 0.02f, 0.16f, 0.10f, 0.05f),
            TRC::LIGHT_R, TRC::LIGHT_G, TRC::LIGHT_B, 80.f, 0.90f);
        _tb(vao, ss, bx(P, 0.86f, 0.92f, zf + 0.02f, 0.16f, 0.10f, 0.05f),
            TRC::LIGHT_R, TRC::LIGHT_G, TRC::LIGHT_B, 80.f, 0.90f);

        _tb(vao, ss, bx(P, -1.06f, 0.92f, zf + 0.02f, 0.08f, 0.08f, 0.05f),
            0.82f, 0.52f, 0.16f, 24.f, 0.50f);
        _tb(vao, ss, bx(P, 1.06f, 0.92f, zf + 0.02f, 0.08f, 0.08f, 0.05f),
            0.82f, 0.52f, 0.16f, 24.f, 0.50f);
    }

    void drawWheel(unsigned int vao, Shader& ss, const glm::mat4& P, glm::vec3 c, float wheelSpinDeg = 0.0f)
    {
        glm::mat4 spinRot = glm::rotate(I, glm::radians(wheelSpinDeg), { 1.f, 0.f, 0.f });
        glm::mat4 axleRot = glm::rotate(I, glm::radians(90.f), { 0.f,0.f,1.f });

        // thick main tyre
        _tc(tyreCyl, ss, P * glm::translate(I, c) * spinRot, glm::vec3(0,0,0),
            WR, WW, WR,
            TRC::RUBBER_R, TRC::RUBBER_G, TRC::RUBBER_B, 20.f, axleRot, 0.10f);

        // inner dark depth
        _tc(tyreCyl, ss, P * glm::translate(I, c) * spinRot, glm::vec3(0,0,0),
            WR * 0.86f, WW * 0.76f, WR * 0.86f,
            0.04f, 0.04f, 0.04f, 8.f, axleRot, 0.03f);

        // rim faces
        _tc(tyreCyl, ss, P * glm::translate(I, c) * spinRot, glm::vec3(-WW * 0.88f, 0.f, 0.f),
            WR * 0.56f, WW * 0.08f, WR * 0.56f,
            TRC::HUB_R, TRC::HUB_G, TRC::HUB_B, 80.f, axleRot, 0.70f);

        _tc(tyreCyl, ss, P * glm::translate(I, c) * spinRot, glm::vec3(WW * 0.88f, 0.f, 0.f),
            WR * 0.56f, WW * 0.08f, WR * 0.56f,
            TRC::HUB_R, TRC::HUB_G, TRC::HUB_B, 80.f, axleRot, 0.70f);

        // center hub
        _tc(tyreCyl, ss, P * glm::translate(I, c) * spinRot, glm::vec3(0,0,0),
            WR * 0.16f, WW * 1.70f, WR * 0.16f,
            0.18f, 0.18f, 0.18f, 16.f, axleRot, 0.15f);

        // jeep-style side tread blocks
        for (int i = 0; i < 14; ++i) {
            float a = glm::radians(i * (360.f / 14.f));
            float y = (WR - 0.03f) * cosf(a);
            float z = (WR - 0.03f) * sinf(a);

            glm::mat4 tread = P * glm::translate(I, c) * spinRot;
            tread = glm::translate(tread, glm::vec3(0.f, y, z));
            tread = glm::rotate(tread, a, glm::vec3(1.f, 0.f, 0.f));
            tread = glm::scale(tread, glm::vec3(WW * 0.98f, 0.06f, 0.18f));

            _tb(vao, ss, tread, 0.14f, 0.14f, 0.14f, 10.f, 0.06f);
        }
    }
};

#endif