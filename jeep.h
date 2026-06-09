#ifndef JEEP_H
#define JEEP_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "cube.h"
#include "cylinder.h"
#include <cmath>

namespace JC {
    constexpr float BR = .78f, BG = .75f, BB = .62f;
    constexpr float DR = .10f, DG = .10f, DB = .10f;
    constexpr float GR = .28f, GG = .36f, GB = .44f;
    constexpr float CR = .55f, CG = .55f, CB = .58f;
    constexpr float TR = .08f, TG = .08f, TB = .09f;
    constexpr float SLR = .22f, SLG = .22f, SLB = .22f;
}

static void _js(unsigned int vao, Shader& sh, const glm::mat4& m,
    float r, float g, float b, float sh2 = 16.f)
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * .28f, g * .28f, b * .28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(.60f, .60f, .60f));
    sh.setFloat("material.shininess", sh2);
    sh.setMat4("model", m);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

static void _jt(Cube& c, Shader& sh, const glm::mat4& m)
{
    c.drawCubeWithTexture(sh, m);
}

static void _jc(Shape& cy, Shader& sh, const glm::mat4& P,
    glm::vec3 pos, float rx, float ry, float rz,
    float r, float g, float b, float sh2 = 24.f,
    const glm::mat4& rot = glm::mat4(1.f))
{
    sh.use();
    sh.setVec3("material.ambient", glm::vec3(r * .28f, g * .28f, b * .28f));
    sh.setVec3("material.diffuse", glm::vec3(r, g, b));
    sh.setVec3("material.specular", glm::vec3(.35f, .35f, .35f));
    sh.setFloat("material.shininess", sh2);

    glm::mat4 I(1.f);
    glm::mat4 m = P * glm::translate(I, pos) * rot * glm::scale(I, glm::vec3(rx, ry, rz));

    cy.drawColor(sh, 0, 0, 0, 0, 0, 0, 1, 1, 1, m, glm::vec3(r, g, b));
}

class Jeep {
public:
    Cube& body;
    Shape& tyreCyl;
    Shape& wheelFaceCyl;

    static constexpr float LB_L = 5.00f;
    static constexpr float LB_W = 2.20f;
    static constexpr float LB_H = 1.40f;

    static constexpr float UC_W = 2.00f;
    static constexpr float UC_H = 1.45f;
    static constexpr float UC_SB = 1.30f;

    static constexpr float WR = 0.65f;
    static constexpr float WW = 0.28f;

    static constexpr float WCX = 1.12f;
    static constexpr float WZF = 1.60f;
    static constexpr float WZR = -1.46f;

    static constexpr float HOOD_L = 1.52f;
    static constexpr float WS_TILT = 18.f;

    Jeep(Cube& b, Shape& tyre, Shape& face)
        : body(b), tyreCyl(tyre), wheelFaceCyl(face) {
    }

    void draw(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P,
              float doorAngleDeg = 0.0f,
              float windowDrop = 0.0f,
              float wheelSpinDeg = 0.0f,
              bool headlightsOn = false)
    {
        drawLowerBody(vao, ss, ts, P);
        drawChassisDepth(vao, ss, P);
        drawHood(vao, ss, ts, P);
        drawUpperCabin(vao, ss, ts, P);
        drawWindshield(vao, ss, P);
        drawRoof(vao, ss, ts, P);
        drawInteriorSeats(vao, ss, P);
        drawDoors(vao, ss, ts, P, doorAngleDeg, windowDrop);
        drawWindows(vao, ss, P);

        drawFrontGrille(vao, ss, P);
        drawHeadlights(ss, P, headlightsOn);
        drawBumpers(vao, ss, P);
        drawWheelArches(vao, ss, ts, P);

        drawWheel(vao, ss, P, { -WCX, WR,  WZF }, wheelSpinDeg);
        drawWheel(vao, ss, P, { WCX, WR,  WZF }, wheelSpinDeg);
        drawWheel(vao, ss, P, { -WCX, WR,  WZR }, wheelSpinDeg);
        drawWheel(vao, ss, P, { WCX, WR,  WZR }, wheelSpinDeg);

        drawSpareTyre(vao, ss, P);
        drawMirrors(vao, ss, P);
        drawExhaust(ss, P);
        drawSmallMilitaryDetails(vao, ss, P);
    }

private:
    glm::mat4 I = glm::mat4(1.f);

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

    void drawLowerBody(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float hw = LB_W * .5f;
        float bodyRaiseY = 0.65f;

        _jt(body, ts, bx(P, 0, bodyRaiseY + LB_H * .5f, 0, LB_W, LB_H, LB_L));

        _js(vao, ss, bx(P, 0, bodyRaiseY + .11f, 0, LB_W + .12f, .22f, LB_L + .08f),
            JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 20.f);

        for (int s : { -1, 1 }) {
            _js(vao, ss, bx(P, s * (hw + .03f), bodyRaiseY + LB_H * .30f, 0,
                .07f, LB_H * .56f, LB_L * .90f),
                JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        }

        for (int s : { -1, 1 }) {
            _js(vao, ss, bx(P, s * (hw + .10f), bodyRaiseY + .19f, -.05f,
                .12f, .07f, LB_L * .64f),
                JC::DR + .10f, JC::DG + .10f, JC::DB + .10f, 24.f);
        }

        _js(vao, ss, bx(P, 0, bodyRaiseY + LB_H - .05f, -.10f,
            LB_W - .06f, .04f, LB_L * .82f),
            .58f, .56f, .46f, 18.f);
    }

    void drawChassisDepth(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float y = 0.28f;

        _js(vao, ss, bx(P, 0.0f, y, 0.02f, 1.82f, .10f, 3.10f),
            JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 10.f);

        _js(vao, ss, bx(P, 0.0f, y + .02f, WZF, 1.95f, .08f, .12f),
            JC::DR + .08f, JC::DG + .08f, JC::DB + .08f, 10.f);
        _js(vao, ss, bx(P, 0.0f, y + .02f, WZR, 1.95f, .08f, .12f),
            JC::DR + .08f, JC::DG + .08f, JC::DB + .08f, 10.f);

        _js(vao, ss, bx(P, 0.0f, y + .03f, WZR + .06f, .28f, .16f, .22f),
            JC::DR + .10f, JC::DG + .10f, JC::DB + .10f, 14.f);
    }

    void drawHood(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float hoodZf = LB_L * .5f;
        float hoodZr = hoodZf - HOOD_L;
        float bodyRaiseY = 0.65f;

        // final hood dimensions
        float hoodW = LB_W - .20f;
        float hoodH = .09f;
        float hoodL = HOOD_L - .45f;

        // place by CENTER, not by cube corner
        glm::vec3 hoodCenter(
            0.f,
            bodyRaiseY + LB_H,
            hoodZr + (HOOD_L * 0.5f) - .05f
        );

        glm::mat4 hood = P
            * glm::translate(I, hoodCenter)
            * glm::rotate(I, glm::radians(-3.0f), { 1,0,0 })
            * glm::translate(I, { -hoodW * 0.5f, -hoodH * 0.5f, -hoodL * 0.5f })
            * glm::scale(I, { hoodW, hoodH, hoodL });
        _jt(body, ts, hood);

        // center ridge, also centered properly
        float ridgeW = .32f;
        float ridgeH = .022f;
        float ridgeL = HOOD_L - .60f;

        glm::mat4 ridge = P
            * glm::translate(I, {
                0.f,
                bodyRaiseY + LB_H + .020f,
                hoodZr + (HOOD_L * 0.5f) - .06f
                })
            * glm::rotate(I, glm::radians(-3.0f), { 1,0,0 })
            * glm::translate(I, { -ridgeW * 0.5f, -ridgeH * 0.5f, -ridgeL * 0.5f })
            * glm::scale(I, { ridgeW, ridgeH, ridgeL });
        _jt(body, ts, ridge);

        // front trim
        _js(vao, ss, bx(P, 0.f, bodyRaiseY + LB_H - .005f, hoodZf - .09f,
            LB_W - .16f, .04f, .03f),
            JC::DR + .07f, JC::DG + .07f, JC::DB + .07f, 18.f);

        // center seam
        _js(vao, ss, bx(P, 0.f, bodyRaiseY + LB_H + .010f,
            hoodZr + (HOOD_L * 0.5f) - .06f,
            .018f, .008f, HOOD_L - .62f),
            JC::DR, JC::DG, JC::DB, 8.f);
    }

    void drawUpperCabin(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float hw = UC_W * .5f;
        float bodyRaiseY = 0.65f;
        float y0 = bodyRaiseY + LB_H;
        float zf = LB_L * .5f - UC_SB;
        float zr = -(LB_L * .5f - .18f);
        float L = zf - zr;
        float wt = .08f;

        float pillarFrontZ = zf - 0.06f;
        float pillarMidZ = zr + L * 0.58f;
        float pillarRearZ = zr + 0.02f;

        float lowerPanelH = UC_H * 0.47f;
        float upperOpenY = y0 + lowerPanelH;
        float topRailY = y0 + UC_H - wt;

        // ONLY CHANGE:
        // rear lower side panel only, so the door opening stays empty
        float rearLowerPanelL = (pillarMidZ - pillarRearZ) + 0.08f;

        _jt(body, ts, bm(P, -hw - wt, y0, pillarRearZ, wt, lowerPanelH, rearLowerPanelL));
        _js(vao, ss, bm(P, -hw - wt, topRailY, zr, wt, wt, L),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, -hw - wt, upperOpenY, pillarFrontZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, -hw - wt, upperOpenY, pillarMidZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, -hw - wt, upperOpenY, pillarRearZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);

        _jt(body, ts, bm(P, hw, y0, pillarRearZ, wt, lowerPanelH, rearLowerPanelL));
        _js(vao, ss, bm(P, hw, topRailY, zr, wt, wt, L),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, hw, upperOpenY, pillarFrontZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, hw, upperOpenY, pillarMidZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        _js(vao, ss, bm(P, hw, upperOpenY, pillarRearZ, wt, UC_H - lowerPanelH, wt),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);

        _jt(body, ts, bm(P, -hw - wt, y0, zr, UC_W + wt * 2.f, UC_H, wt));

        _js(vao, ss, bm(P, -hw - wt, y0 - .02f, zr, UC_W + wt * 2.f, .06f, L),
            JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 12.f);
    }
    void drawWindshield(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float hw = UC_W * .5f;
        float bodyRaiseY = 0.65f;
        float y0 = bodyRaiseY + LB_H;
        float zf = LB_L * .5f - UC_SB;
        float wt = .08f;
        glm::mat4 rotWS = glm::rotate(I, glm::radians(-WS_TILT), { 1,0,0 });

        {
            glm::mat4 m = P
                * glm::translate(I, { -(hw + wt + .01f), y0 + .02f, zf + .03f })
                * rotWS
                * glm::scale(I, { UC_W + wt * 2 + .02f, UC_H - .02f, .08f });
            _js(vao, ss, m, JC::DR + .06f, JC::DG + .06f, JC::DB + .06f, 20.f);
        }

        {
            glm::mat4 m = P
                * glm::translate(I, { -(hw - .11f), y0 + .13f, zf + .035f })
                * rotWS
                * glm::scale(I, { UC_W - .22f, UC_H - .30f, .035f });
            _js(vao, ss, m, .42f, .48f, .56f, 36.f);
        }

        {
            glm::mat4 m = P
                * glm::translate(I, { -(hw - .10f), y0 + .14f, zf - .01f })
                * rotWS
                * glm::scale(I, { UC_W - .26f, UC_H - .36f, .025f });
            _js(vao, ss, m, .12f, .14f, .16f, 8.f);
        }

        for (int s : { -1, 1 }) {
            glm::mat4 m = P
                * glm::translate(I, { s * (hw + wt * .5f - .02f), y0, zf + .04f })
                * rotWS
                * glm::scale(I, { .09f, UC_H, .09f });
            _js(vao, ss, m, JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);
        }
    }

    void drawRoof(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float hw = UC_W * .5f;
        float bodyRaiseY = 0.65f;
        float ry = bodyRaiseY + LB_H + UC_H;
        float zf = LB_L * .5f - UC_SB;
        float zr = -(LB_L * .5f - .18f);
        float L = zf - zr;
        float wt = .08f;

        _jt(body, ts, bm(P, -hw - wt, ry - .04f, zr, UC_W + wt * 2.f, .08f, L));

        _js(vao, ss, bm(P, -hw - wt - .02f, ry - .01f, zr, .035f, .04f, L),
            JC::DR + .07f, JC::DG + .07f, JC::DB + .07f, 20.f);
        _js(vao, ss, bm(P, hw + wt - .015f, ry - .01f, zr, .035f, .04f, L),
            JC::DR + .07f, JC::DG + .07f, JC::DB + .07f, 20.f);
    }

    void drawDoors(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P, float doorAngleDeg, float windowDrop)
    {
        float hw = UC_W * .5f;
        float bodyRaiseY = 0.65f;
        float y0 = bodyRaiseY + LB_H;
        float wt = .08f;

        float zf = LB_L * .5f - UC_SB - .10f;     // front edge of door
        float zr = -(LB_L * .5f - .58f);          // rear edge of door
        float dL = zf - zr;

        float lowerH = UC_H * .47f;
        float upperH = UC_H - lowerH;

        const float darkTint = 0.72f;

        for (int s : { -1, 1 }) {
            float x = (s < 0) ? (-hw - wt - .01f) : (hw + .01f);
            float doorT = .045f;

            // hinge at the front vertical edge
            glm::vec3 hinge = glm::vec3(x, y0, zf);

            glm::mat4 doorRoot = P;
            doorRoot = glm::translate(doorRoot, hinge);
            doorRoot = glm::rotate(
                doorRoot,
                glm::radians((s < 0 ? 1.0f : -1.0f) * doorAngleDeg),
                glm::vec3(0, 1, 0)
            );
            doorRoot = glm::translate(doorRoot, -hinge);

            // ===== DOUBLE-SKIN TEXTURED DOOR =====
            // center slab
            glm::mat4 doorCore = doorRoot
                * glm::translate(I, glm::vec3(x, y0, zr))
                * glm::scale(I, glm::vec3(doorT, lowerH, dL));
            _jt(body, ts, doorCore);

            // outer textured skin
            glm::mat4 outerSkin = doorRoot
                * glm::translate(I, glm::vec3(
                    x + (s < 0 ? -0.006f : 0.006f),
                    y0 + 0.01f,
                    zr + 0.01f))
                * glm::scale(I, glm::vec3(.006f, lowerH - 0.02f, dL - 0.02f));
            _jt(body, ts, outerSkin);

            // inner textured skin
            glm::mat4 innerSkin = doorRoot
                * glm::translate(I, glm::vec3(
                    x + (s < 0 ? 0.001f : -0.001f),
                    y0 + 0.01f,
                    zr + 0.01f))
                * glm::scale(I, glm::vec3(.006f, lowerH - 0.02f, dL - 0.02f));
            _jt(body, ts, innerSkin);

            // top door frame
            glm::mat4 topStrip = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + lowerH + upperH - .07f, zr))
                * glm::scale(I, glm::vec3(.045f, .07f, dL));
            _js(vao, ss, topStrip,
                (JC::DR + .05f) * darkTint,
                (JC::DG + .05f) * darkTint,
                (JC::DB + .05f) * darkTint, 18.f);

            // front vertical frame
            glm::mat4 frontFrame = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + lowerH, zf - .07f))
                * glm::scale(I, glm::vec3(.045f, upperH, .07f));
            _js(vao, ss, frontFrame,
                (JC::DR + .05f) * darkTint,
                (JC::DG + .05f) * darkTint,
                (JC::DB + .05f) * darkTint, 18.f);

            // rear vertical frame
            glm::mat4 rearFrame = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + lowerH, zr))
                * glm::scale(I, glm::vec3(.045f, upperH, .07f));
            _js(vao, ss, rearFrame,
                (JC::DR + .05f) * darkTint,
                (JC::DG + .05f) * darkTint,
                (JC::DB + .05f) * darkTint, 18.f);

            // mid strip
            glm::mat4 midStrip = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + lowerH - .012f, zr))
                * glm::scale(I, glm::vec3(.045f, .024f, dL));
            _js(vao, ss, midStrip,
                JC::DR * darkTint, JC::DG * darkTint, JC::DB * darkTint, 8.f);

            // outer thin line
            glm::mat4 outerLine = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + .08f, zf - .22f))
                * glm::scale(I, glm::vec3(.045f, UC_H - .16f, .018f));
            _js(vao, ss, outerLine,
                JC::DR * darkTint, JC::DG * darkTint, JC::DB * darkTint, 8.f);

            // handle
            glm::mat4 handle = doorRoot
                * glm::translate(I, glm::vec3(x, y0 + lowerH * .55f, zr + dL * .68f))
                * glm::scale(I, glm::vec3(.065f, .045f, .16f));
            _js(vao, ss, handle,
                JC::CR * darkTint, JC::CG * darkTint, JC::CB * darkTint, 70.f);

            // ===== DOOR WINDOW ATTACHED TO DOOR =====
            float maxGlassH = (UC_H - lowerH - .13f);
            float glassH = maxGlassH - windowDrop;

            // thin frame border only
            glm::mat4 doorGlassFrameTop = doorRoot
                * glm::translate(I, glm::vec3(
                    x,
                    y0 + lowerH + maxGlassH + .015f,
                    zr + .06f))
                * glm::scale(I, glm::vec3(.045f, .018f, dL - .12f));
            _js(vao, ss, doorGlassFrameTop, JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 12.f);

            glm::mat4 doorGlassFrameFront = doorRoot
                * glm::translate(I, glm::vec3(
                    x,
                    y0 + lowerH + .03f,
                    zf - .07f))
                * glm::scale(I, glm::vec3(.045f, maxGlassH, .018f));
            _js(vao, ss, doorGlassFrameFront, JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 12.f);

            glm::mat4 doorGlassFrameRear = doorRoot
                * glm::translate(I, glm::vec3(
                    x,
                    y0 + lowerH + .03f,
                    zr + .06f))
                * glm::scale(I, glm::vec3(.045f, maxGlassH, .018f));
            _js(vao, ss, doorGlassFrameRear, JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 12.f);

            // draw glass only if still visible
            if (glassH > 0.03f) {
                float glassY = y0 + lowerH + .05f - (windowDrop * 0.5f);

                glm::mat4 doorGlass = doorRoot
                    * glm::translate(I, glm::vec3(
                        x + (s < 0 ? -0.020f : 0.020f),
                        glassY,
                        zr + 0.08f))
                    * glm::scale(I, glm::vec3(.020f, glassH, dL - .16f));
                _js(vao, ss, doorGlass, .22f, .28f, .34f, 28.f);
            }
        }
    }

    void drawWindows(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float hw = UC_W * .5f;
        float bodyRaiseY = 0.65f;
        float y0 = bodyRaiseY + LB_H;

        float lowerH = UC_H * .47f;
        float winY = y0 + lowerH + .05f;
        float winH = UC_H - lowerH - .13f;

        // only rear window stays here
        float rearZ = -(LB_L * .5f - .18f) - .02f;
        _js(vao, ss, bm(P, -hw + .12f, winY, rearZ, UC_W - .24f, winH, .022f),
            .20f, .25f, .31f, 30.f);

        _js(vao, ss, bm(P, -hw + .10f, winY - .02f, rearZ - .008f, UC_W - .20f, winH + .04f, .012f),
            JC::DR + .04f, JC::DG + .04f, JC::DB + .04f, 12.f);
    }

    void drawInteriorSeats(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float bodyRaiseY = 0.65f;

        // 🔥 FIX: raise seats higher so visible
        float seatBaseY = bodyRaiseY + 0.75f;

        // 🔥 FIX: move seats slightly forward (toward front opening)
        float seatZ = 0.10f;

        // =========================
        // FRONT-LEFT SEAT
        // =========================

        _js(vao, ss, bx(P, -0.45f, seatBaseY, seatZ, 0.42f, 0.14f, 0.52f),
            0.26f, 0.26f, 0.26f, 18.f);

        _js(vao, ss, bx(P, -0.45f, seatBaseY + 0.34f, seatZ - 0.10f, 0.20f, 0.60f, 0.45f),
            0.22f, 0.22f, 0.22f, 18.f);

        _js(vao, ss, bx(P, -0.45f, seatBaseY + 0.70f, seatZ - 0.10f, 0.14f, 0.14f, 0.14f),
            0.24f, 0.24f, 0.24f, 18.f);

        _js(vao, ss, bx(P, -0.45f, seatBaseY - 0.15f, seatZ, 0.28f, 0.05f, 0.36f),
            0.08f, 0.08f, 0.08f, 10.f);

        // =========================
        // FRONT-RIGHT SEAT
        // =========================

        _js(vao, ss, bx(P, 0.45f, seatBaseY, seatZ, 0.42f, 0.14f, 0.52f),
            0.26f, 0.26f, 0.26f, 18.f);

        _js(vao, ss, bx(P, 0.45f, seatBaseY + 0.34f, seatZ - 0.10f, 0.20f, 0.60f, 0.45f),
            0.22f, 0.22f, 0.22f, 18.f);

        _js(vao, ss, bx(P, 0.45f, seatBaseY + 0.70f, seatZ - 0.10f, 0.14f, 0.14f, 0.14f),
            0.24f, 0.24f, 0.24f, 18.f);

        _js(vao, ss, bx(P, 0.45f, seatBaseY - 0.15f, seatZ, 0.28f, 0.05f, 0.36f),
            0.08f, 0.08f, 0.08f, 10.f);
    }

    void drawFrontGrille(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float bodyRaiseY = 0.65f;
        float gZ = LB_L * .5f + .015f;
        float gY0 = bodyRaiseY + 0.20f;
        float gH = 0.58f;
        float gW = LB_W * .56f;

        _js(vao, ss, bx(P, 0.0f, gY0, gZ, gW + .18f, gH + .08f, .16f),
            JC::DR + .05f, JC::DG + .05f, JC::DB + .05f, 18.f);

        _js(vao, ss, bx(P, 0.0f, gY0 + .01f, gZ + .010f, gW, gH, .08f),
            JC::DR + .01f, JC::DG + .01f, JC::DB + .01f, 12.f);

        float sw = gW / (7 * 2.f - 1.f);
        for (int i = 0; i < 7; i++) {
            float sx = -gW * .5f + i * sw * 2.f;
            _js(vao, ss, bx(P, sx, gY0 + .02f, gZ + .025f, sw, gH - .05f, .025f),
                JC::SLR + .18f, JC::SLG + .18f, JC::SLB + .18f, 80.f);
        }

        _js(vao, ss, bx(P, 0.0f, gY0 + gH * .5f + .18f, gZ + .005f, gW + .22f, .05f, .07f),
            JC::DR + .08f, JC::DG + .08f, JC::DB + .08f, 20.f);

        _js(vao, ss, bx(P, 0.0f, bodyRaiseY + .10f, LB_L * .5f + .145f, .42f, .08f, .05f),
            JC::DR + .18f, JC::DG + .18f, JC::DB + .18f, 14.f);
    }

    void drawHeadlights(Shader& ss, const glm::mat4& P, bool headlightsOn)
    {
        float bodyRaiseY = 0.65f;
        float hz = LB_L * .5f + .045f;
        float hy = bodyRaiseY + 0.47f;
        float hxs[] = { -LB_W * .5f + .28f, LB_W * .5f - .28f };
        glm::mat4 rx = glm::rotate(I, glm::radians(90.f), { 1,0,0 });

        float glassR = headlightsOn ? 0.95f : 0.25f;
        float glassG = headlightsOn ? 0.95f : 0.25f;
        float glassB = headlightsOn ? 0.82f : 0.25f;

        float glowR = headlightsOn ? 1.00f : 0.10f;
        float glowG = headlightsOn ? 0.98f : 0.10f;
        float glowB = headlightsOn ? 0.85f : 0.10f;

        for (float hx : hxs) {
            _jc(tyreCyl, ss, P, { hx, hy, hz }, .24f, .12f, .24f,
                JC::DR, JC::DG, JC::DB, 10.f, rx);

            _jc(tyreCyl, ss, P, { hx, hy, hz + .018f }, .19f, .026f, .19f,
                .55f, .55f, .58f, 80.f, rx);

            _jc(tyreCyl, ss, P, { hx, hy + .005f, hz + .040f }, .16f, .028f, .16f,
                glassR, glassG, glassB, headlightsOn ? 180.f : 18.f, rx);

            _jc(tyreCyl, ss, P, { hx + (hx < 0 ? -.12f : .12f), hy - .08f, hz + .02f },
                .045f, .018f, .045f, .82f, .52f, .18f, 40.f, rx);

            if (headlightsOn) {
                _jc(tyreCyl, ss, P, { hx, hy + .005f, hz + .065f }, .11f, .010f, .11f,
                    glowR, glowG, glowB, 220.f, rx);
            }
        }
    }

    void drawBumpers(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float bodyRaiseY = 0.65f;

        _js(vao, ss, bx(P, 0.0f, bodyRaiseY + 0.11f, LB_L * 0.5f + 0.10f,
            LB_W + 0.02f, 0.10f, 0.16f),
            JC::DR + .12f, JC::DG + .12f, JC::DB + .12f, 18.f);

        _js(vao, ss, bx(P, -0.52f, bodyRaiseY + 0.08f, LB_L * 0.5f + 0.135f, .08f, .08f, .08f),
            JC::DR + .14f, JC::DG + .14f, JC::DB + .14f, 16.f);
        _js(vao, ss, bx(P, 0.52f, bodyRaiseY + 0.08f, LB_L * 0.5f + 0.135f, .08f, .08f, .08f),
            JC::DR + .14f, JC::DG + .14f, JC::DB + .14f, 16.f);

        _js(vao, ss, bx(P, 0.0f, bodyRaiseY + 0.07f, LB_L * 0.5f + 0.17f, .10f, .05f, .05f),
            JC::DR + .20f, JC::DG + .20f, JC::DB + .20f, 16.f);

        _js(vao, ss, bx(P, 0.0f, bodyRaiseY + 0.12f, -(LB_L * 0.5f + 0.08f),
            LB_W + 0.02f, 0.10f, 0.16f),
            JC::DR + .12f, JC::DG + .12f, JC::DB + .12f, 18.f);
    }

    void drawWheelArches(unsigned int vao, Shader& ss, Shader& ts, const glm::mat4& P)
    {
        float sideT = .10f;
        float bodyRaiseY = 0.65f;
        float archY = bodyRaiseY + LB_H * 0.36f;
        float wheelZs[] = { WZF, WZR };

        for (float z : wheelZs) {
            for (int s : { -1, 1 }) {
                float x = (s < 0) ? -(LB_W * .5f + .02f) : (LB_W * .5f - sideT + .02f);

                _jt(body, ts, bm(P, x, archY - .12f, z - .48f, sideT, .34f, .96f));

                _js(vao, ss, bm(P, x + (s < 0 ? -.005f : .005f), archY + .38f, z - .40f,
                    sideT + .02f, .08f, .80f),
                    .46f, .45f, .38f, 18.f);

                _js(vao, ss, bm(P, x, archY + .14f, z + .27f, sideT + .02f, .22f, .16f),
                    .44f, .43f, .36f, 18.f);
                _js(vao, ss, bm(P, x, archY + .28f, z + .14f, sideT + .02f, .14f, .18f),
                    .45f, .44f, .37f, 18.f);

                _js(vao, ss, bm(P, x, archY + .14f, z - .43f, sideT + .02f, .22f, .16f),
                    .44f, .43f, .36f, 18.f);
                _js(vao, ss, bm(P, x, archY + .28f, z - .28f, sideT + .02f, .14f, .18f),
                    .45f, .44f, .37f, 18.f);

                _js(vao, ss, bm(P, x + (s < 0 ? -.01f : .01f), archY + .05f, z - .42f,
                    .03f, .52f, .84f),
                    JC::DR + .03f, JC::DG + .03f, JC::DB + .03f, 8.f);
            }
        }
    }

    void drawWheel(unsigned int vao, Shader& ss, const glm::mat4& P, glm::vec3 centre, float wheelSpinDeg)
    {
        glm::mat4 spinRot = glm::rotate(I, glm::radians(wheelSpinDeg), { 1.f, 0.f, 0.f });
        glm::mat4 axleRot = glm::rotate(I, glm::radians(90.f), { 0.f, 0.f, 1.f });
        glm::mat4 wheelBase = P * glm::translate(I, centre) * spinRot;

        const float tyreR = WR;
        const float tyreHW = WW;

        const float bodyR1 = tyreR * 1.00f;
        const float bodyR2 = tyreR * 0.93f;

        const float facePlateR = tyreR * 0.88f;
        const float boltRingR = tyreR * 0.34f;
        const float boltR = tyreR * 0.070f;
        const float centerR = tyreR * 0.110f;
        const float centerRingR = tyreR * 0.155f;

        const float faceX = tyreHW * 0.92f;
        const float ringX = tyreHW * 0.86f;
        const float boltX = tyreHW * 0.98f;
        const float centerX = tyreHW * 1.00f;

        _jc(tyreCyl, ss, wheelBase, glm::vec3(0,0,0),
            bodyR1, tyreHW * 1.55f, bodyR1,
            0.10f, 0.10f, 0.10f, 26.f, axleRot);

        _jc(tyreCyl, ss, wheelBase, glm::vec3(0,0,0),
            bodyR2, tyreHW * 1.15f, bodyR2,
            0.14f, 0.14f, 0.14f, 24.f, axleRot);

        _jc(tyreCyl, ss, wheelBase,
            glm::vec3(-tyreHW * 0.42f, 0.f, 0.f),
            tyreR * 0.98f, tyreHW * 0.62f, tyreR * 0.98f,
            0.12f, 0.12f, 0.12f, 28.f, axleRot);

        _jc(tyreCyl, ss, wheelBase,
            glm::vec3(tyreHW * 0.42f, 0.f, 0.f),
            tyreR * 0.98f, tyreHW * 0.62f, tyreR * 0.98f,
            0.12f, 0.12f, 0.12f, 28.f, axleRot);

        {
            const int lugCount = 14;
            const float lugRingR = tyreR * 0.93f;
            const float lugW = tyreHW * 0.22f;
            const float lugH = 0.075f;
            const float lugD = 0.18f;

            for (int i = 0; i < lugCount; i++) {
                float a = glm::radians(i * (360.f / lugCount));
                float y = lugRingR * cosf(a);
                float z = lugRingR * sinf(a);

                glm::mat4 lugL = wheelBase;
                lugL = glm::translate(lugL, glm::vec3(-tyreHW * 0.80f, y, z));
                lugL = glm::rotate(lugL, a, glm::vec3(1.f, 0.f, 0.f));
                lugL = glm::scale(lugL, glm::vec3(lugW, lugH, lugD));
                _js(vao, ss, lugL, 0.18f, 0.18f, 0.18f, 18.f);

                glm::mat4 lugR = wheelBase;
                lugR = glm::translate(lugR, glm::vec3(tyreHW * 0.80f, y, z));
                lugR = glm::rotate(lugR, a, glm::vec3(1.f, 0.f, 0.f));
                lugR = glm::scale(lugR, glm::vec3(lugW, lugH, lugD));
                _js(vao, ss, lugR, 0.18f, 0.18f, 0.18f, 18.f);
            }
        }

        {
            const int treadCount = 16;
            for (int i = 0; i < treadCount; i++) {
                float a = glm::radians(i * (360.f / treadCount));
                float y = (tyreR - 0.03f) * cosf(a);
                float z = (tyreR - 0.03f) * sinf(a);

                glm::mat4 tread = wheelBase;
                tread = glm::translate(tread, glm::vec3(0.f, y, z));
                tread = glm::rotate(tread, a, glm::vec3(1.f, 0.f, 0.f));
                tread = glm::scale(tread, glm::vec3(tyreHW * 0.95f, 0.050f, 0.18f));

                _js(vao, ss, tread, 0.20f, 0.20f, 0.20f, 18.f);
            }
        }

        _jc(tyreCyl, ss, wheelBase, glm::vec3(-faceX, 0.f, 0.f),
            facePlateR, tyreHW * 0.11f, facePlateR,
            0.09f, 0.09f, 0.09f, 20.f, axleRot);

        _jc(tyreCyl, ss, wheelBase, glm::vec3(-ringX, 0.f, 0.f),
            centerRingR, tyreHW * 0.05f, centerRingR,
            0.18f, 0.18f, 0.20f, 50.f, axleRot);

        _jc(tyreCyl, ss, wheelBase, glm::vec3(-centerX, 0.f, 0.f),
            centerR, tyreHW * 0.10f, centerR,
            0.30f, 0.30f, 0.34f, 80.f, axleRot);

        for (int k = 0; k < 6; k++) {
            float a = glm::radians(k * 60.f);
            float by = boltRingR * cosf(a);
            float bz = boltRingR * sinf(a);

            _jc(tyreCyl, ss, wheelBase,
                glm::vec3(-boltX, -by, -bz),
                boltR, tyreHW * 0.09f, boltR,
                0.72f, 0.72f, 0.76f, 120.f, axleRot);
        }

        _jc(tyreCyl, ss, wheelBase, glm::vec3(faceX, 0.f, 0.f),
            facePlateR, tyreHW * 0.11f, facePlateR,
            0.09f, 0.09f, 0.09f, 20.f, axleRot);

        _jc(tyreCyl, ss, wheelBase, glm::vec3(ringX, 0.f, 0.f),
            centerRingR, tyreHW * 0.05f, centerRingR,
            0.18f, 0.18f, 0.20f, 50.f, axleRot);

        _jc(tyreCyl, ss, wheelBase, glm::vec3(centerX, 0.f, 0.f),
            centerR, tyreHW * 0.10f, centerR,
            0.30f, 0.30f, 0.34f, 80.f, axleRot);

        for (int k = 0; k < 6; k++) {
            float a = glm::radians(k * 60.f);
            float by = boltRingR * cosf(a);
            float bz = boltRingR * sinf(a);

            _jc(tyreCyl, ss, wheelBase,
                glm::vec3(boltX, by, bz),
                boltR, tyreHW * 0.09f, boltR,
                0.72f, 0.72f, 0.76f, 120.f, axleRot);
        }

        _jc(tyreCyl, ss, wheelBase, glm::vec3(0,0,0),
            0.045f, tyreHW * 2.20f, 0.045f,
            0.22f, 0.22f, 0.24f, 24.f, axleRot);
    }

    void drawSpareTyre(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float bodyRaiseY = 0.65f;
        float sz = -(LB_L * 0.5f + 0.16f);
        float sy = bodyRaiseY + LB_H + UC_H * 0.45f;

        glm::mat4 axleRot = glm::rotate(I, glm::radians(90.f), { 1, 0, 0 });

        float tyreR = WR * 0.78f;
        float tyreHalfW = WW * 1.25f;
        float rimR = tyreR * 0.58f;
        float rimInnerR = tyreR * 0.35f;
        float hubR = tyreR * 0.12f;

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz },
            tyreR, tyreHalfW, tyreR,
            0.12f, 0.12f, 0.12f, 24.f, axleRot);

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + 0.02f },
            tyreR * 0.92f, tyreHalfW * 0.94f, tyreR * 0.92f,
            0.18f, 0.18f, 0.18f, 30.f, axleRot);

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + tyreHalfW * 0.72f },
            rimR, tyreHalfW * 0.08f, rimR,
            0.62f, 0.62f, 0.65f, 96.f, axleRot);

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + tyreHalfW * 0.68f },
            rimInnerR, tyreHalfW * 0.05f, rimInnerR,
            0.22f, 0.22f, 0.24f, 16.f, axleRot);

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + tyreHalfW * 0.70f },
            rimInnerR * 0.85f, tyreHalfW * 0.10f, rimInnerR * 0.85f,
            0.06f, 0.06f, 0.07f, 8.f, axleRot);

        float spokeCount = 4.0f;
        float spokeLen = rimR - rimInnerR * 0.85f - 0.03f;
        float spokeR = tyreR * 0.035f;

        for (int i = 0; i < 4; i++) {
            float angle = glm::radians(i * (360.0f / spokeCount));

            glm::mat4 spinX = glm::rotate(I, angle, glm::vec3(1, 0, 0));
            glm::mat4 spokeLocal =
                glm::translate(I, glm::vec3(0.f, spokeLen * 0.5f + rimInnerR * 0.85f, 0.f))
                * glm::scale(I, glm::vec3(spokeR, spokeLen * 0.5f, spokeR));

            glm::mat4 m = P
                * glm::translate(I, glm::vec3(0.0f, sy, sz))
                * axleRot
                * spinX
                * spokeLocal;

            ss.use();
            ss.setVec3("material.ambient", glm::vec3(0.22f, 0.22f, 0.24f));
            ss.setVec3("material.diffuse", glm::vec3(0.62f, 0.62f, 0.65f));
            ss.setVec3("material.specular", glm::vec3(0.90f, 0.90f, 0.90f));
            ss.setFloat("material.shininess", 128.f);

            tyreCyl.drawColor(ss, 0, 0, 0, 0, 0, 0, 1, 1, 1, m, glm::vec3(0.62f, 0.62f, 0.65f));
        }

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + tyreHalfW * 0.90f },
            hubR * 1.2f, tyreHalfW * 0.10f, hubR * 1.2f,
            0.80f, 0.80f, 0.85f, 128.f, axleRot);

        _jc(tyreCyl, ss, P, { 0.0f, sy, sz + tyreHalfW * 0.88f },
            hubR * 1.45f, tyreHalfW * 0.04f, hubR * 1.45f,
            0.18f, 0.18f, 0.20f, 16.f, axleRot);
    }

    void drawMirrors(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float hw = UC_W * .5f, wt = .08f;
        float mZ = LB_L * .5f - UC_SB + .06f;
        float bodyRaiseY = 0.65f;
        float y0 = bodyRaiseY + LB_H;

        for (int s : { -1, 1 }) {
            float mx = s * (hw + wt + .13f);

            _js(vao, ss, bx(P, mx, y0 + UC_H * .73f, mZ - .01f, .035f, .035f, .12f),
                JC::DR + .08f, JC::DG + .08f, JC::DB + .08f, 20.f);

            _js(vao, ss, bx(P, mx, y0 + UC_H * .67f, mZ - .05f, .18f, .12f, .05f),
                .24f, .28f, .34f, 32.f);
        }
    }

    void drawExhaust(Shader& ss, const glm::mat4& P)
    {
        glm::mat4 axR = glm::rotate(I, glm::radians(90.f), { 1,0,0 });
        _jc(tyreCyl, ss, P, { LB_W * .43f, .26f, -(LB_L * .5f + .02f) },
            .045f, .28f, .045f,
            JC::DR + .20f, JC::DG + .20f, JC::DB + .20f, 16.f, axR);
    }

    void drawSmallMilitaryDetails(unsigned int vao, Shader& ss, const glm::mat4& P)
    {
        float bodyRaiseY = 0.65f;

        _js(vao, ss, bx(P, -LB_W * .5f - .12f, bodyRaiseY + .72f, -(LB_L * .5f - .48f),
            .14f, .32f, .26f),
            .32f, .36f, .26f, 18.f);

        _js(vao, ss, bx(P, LB_W * .5f + .02f, bodyRaiseY + .90f, -.10f,
            .04f, .05f, .80f),
            JC::DR + .16f, JC::DG + .16f, JC::DB + .16f, 14.f);

        _js(vao, ss, bx(P, -LB_W * .34f, bodyRaiseY + LB_H + UC_H + .38f, -(LB_L * .5f - .34f),
            .02f, .70f, .02f),
            JC::DR + .16f, JC::DG + .16f, JC::DB + .16f, 24.f);
    }
};

#endif