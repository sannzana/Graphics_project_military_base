#pragma once
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <string>
#include "shader.h"
#include "cube.h"
#include "cylinder.h"


static void cf_drawCube(unsigned int cubeVAO, Shader& shader,
    glm::mat4 model,
    float r, float g, float b, float a = 1.0f)
{
    shader.use();
    shader.setVec3("material.ambient", glm::vec3(r, g, b));
    shader.setVec3("material.diffuse", glm::vec3(r, g, b));
    shader.setVec3("material.specular", glm::vec3(0.3f, 0.3f, 0.3f));
    shader.setFloat("material.shininess", 8.0f);
    shader.setMat4("model", model);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


// ---------------------------------------------------------------
// Campfire class
// ---------------------------------------------------------------
class Campfire
{
public:
    // --- point-light state (flickers) ---
    glm::vec3 lightPos;          // world position of the flame centre
    float     flickerIntensity;  // updated every draw call

    // pebble texture IDs (pass from main)
    unsigned int pebbleDiff;
    unsigned int pebbleSpec;

    // ctor  – pass the loaded texture IDs for pebble.png
    Campfire(unsigned int pebbleDiffMap, unsigned int pebbleSpecMap)
        : pebbleDiff(pebbleDiffMap), pebbleSpec(pebbleSpecMap),
        lightPos(0.0f), flickerIntensity(1.0f)
    {
        buildPebblePositions();
    }

    // -----------------------------------------------------------------
    //  Main draw call
    //    parentModel : the transform that places the campfire in world
    //    t           : glfwGetTime() for animation
    // -----------------------------------------------------------------
    void draw(Shader& solidShader,
        Shader& textureShader,
        unsigned int cubeVAO,
        const glm::mat4& parentModel,
        float t)
    {
        glm::mat4 I = glm::mat4(1.0f);

        // --- compute flicker ---
        float flicker = 0.85f
            + 0.08f * sinf(t * 7.3f)
            + 0.07f * sinf(t * 13.1f + 1.2f);
        flickerIntensity = flicker;

        // world position of flame (for point light)
        glm::vec4 wp = parentModel * glm::vec4(0.0f, 0.6f, 0.0f, 1.0f);
        lightPos = glm::vec3(wp);

        // ---- 1. LOGS ----
        drawLogs(solidShader, cubeVAO, parentModel, I);

        // ---- 2. PEBBLES ----
        drawPebbles(textureShader, cubeVAO, parentModel, I);

        // ---- 3. FIRE ----
        drawFire(solidShader, cubeVAO, parentModel, I, t, flicker);
    }

    // -----------------------------------------------------------------
    //  Set up the campfire point light (call for EACH shader you use)
    //  lightNumber: choose a unique index not used by your other lights
    // -----------------------------------------------------------------
    void setUpPointLight(Shader& shader, int lightNumber = 21)
    {
        std::string base = "pointLights[" + std::to_string(lightNumber - 1) + "].";

        // warm orange flicker
        float r = 1.0f * flickerIntensity;
        float g = 0.45f * flickerIntensity;
        float b = 0.05f * flickerIntensity;

        shader.use();
        shader.setVec3(base + "position", lightPos);
        shader.setVec3(base + "ambient", glm::vec3(r * 0.3f, g * 0.3f, b * 0.1f));
        shader.setVec3(base + "diffuse", glm::vec3(r, g, b));
        shader.setVec3(base + "specular", glm::vec3(r * 0.5f, g * 0.2f, 0.0f));
        shader.setFloat(base + "k_c", 1.0f);
        shader.setFloat(base + "k_l", 0.22f);
        shader.setFloat(base + "k_q", 0.20f);
    }

private:
    // pebble ring data (generated once in ctor)
    struct PebbleInfo { float angle; float dist; float sx, sy, sz; float ry; };
    std::vector<PebbleInfo> pebbles;

    void buildPebblePositions()
    {
        // 14 pebbles at varying angles / distances / sizes
        int N = 14;
        for (int i = 0; i < N; i++)
        {
            float a = (float)i / N * 360.0f + (i % 3) * 8.0f;  // angle deg
            float d = 1.1f + 0.25f * ((i % 4) * 0.15f);        // distance from centre
            float sx = 0.18f + 0.10f * (i % 3);
            float sy = 0.12f + 0.08f * (i % 2);
            float sz = 0.16f + 0.09f * ((i + 1) % 3);
            float ry = a * 0.7f;
            pebbles.push_back({ a, d, sx, sy, sz, ry });
        }
    }

    // ------------------------------------------------------------------
    void drawLogs(Shader& shader, unsigned int vao,
        const glm::mat4& parent, const glm::mat4& I)
    {
        // 6 logs arranged like spokes of a wheel, tilted slightly inward
        // Enhanced wood colors with better contrast
        float logR = 0.25f, logG = 0.14f, logB = 0.06f;      // richer brown
        float barkR = 0.08f, barkG = 0.04f, barkB = 0.01f;  // dark charred bark

        int numLogs = 6;
        for (int i = 0; i < numLogs; i++)
        {
            float angleDeg = i * (360.0f / numLogs);
            float angleRad = glm::radians(angleDeg);

            // Each log: long thin box, rotated around Y then tilted inward
            glm::mat4 rotY = glm::rotate(I, angleRad, glm::vec3(0, 1, 0));
            glm::mat4 rotZ = glm::rotate(I, glm::radians(-18.0f), glm::vec3(0, 0, 1));
            // translate along local X so it sits from centre outward
            glm::mat4 trans = glm::translate(I, glm::vec3(0.55f, 0.10f, 0.0f));
            glm::mat4 scale = glm::scale(I, glm::vec3(1.2f, 0.14f, 0.14f));

            glm::mat4 m = parent * rotY * rotZ * trans * scale;
            
            // Main wood body - more realistic
            shader.use();
            shader.setVec3("material.ambient", glm::vec3(logR * 0.4f, logG * 0.4f, logB * 0.4f));
            shader.setVec3("material.diffuse", glm::vec3(logR, logG, logB));
            shader.setVec3("material.specular", glm::vec3(0.15f, 0.12f, 0.08f));
            shader.setFloat("material.shininess", 12.0f);
            shader.setMat4("model", m);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            // Darker wood grain stripe for realism
            glm::mat4 scale2 = glm::scale(I, glm::vec3(1.2f, 0.05f, 0.14f));
            glm::mat4 trans2 = glm::translate(I, glm::vec3(0.55f, 0.16f, 0.0f));
            glm::mat4 m2 = parent * rotY * rotZ * trans2 * scale2;
            
            shader.use();
            shader.setVec3("material.ambient", glm::vec3(barkR * 0.4f, barkG * 0.4f, barkB * 0.4f));
            shader.setVec3("material.diffuse", glm::vec3(barkR, barkG, barkB));
            shader.setVec3("material.specular", glm::vec3(0.05f, 0.03f, 0.01f));
            shader.setFloat("material.shininess", 6.0f);
            shader.setMat4("model", m2);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

            // Light wood stripe for texture variation
            glm::mat4 scale3 = glm::scale(I, glm::vec3(1.2f, 0.02f, 0.14f));
            glm::mat4 trans3 = glm::translate(I, glm::vec3(0.55f, 0.08f, 0.0f));
            glm::mat4 m3 = parent * rotY * rotZ * trans3 * scale3;
            
            shader.use();
            shader.setVec3("material.ambient", glm::vec3(0.30f, 0.18f, 0.10f));
            shader.setVec3("material.diffuse", glm::vec3(0.38f, 0.22f, 0.12f));
            shader.setVec3("material.specular", glm::vec3(0.12f, 0.10f, 0.06f));
            shader.setFloat("material.shininess", 10.0f);
            shader.setMat4("model", m3);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // 3 logs on top (elevated, cross-stacked) - thicker/richer wood
        for (int i = 0; i < 3; i++)
        {
            float angleDeg = i * 120.0f + 60.0f;
            float angleRad = glm::radians(angleDeg);
            glm::mat4 rotY = glm::rotate(I, angleRad, glm::vec3(0, 1, 0));
            glm::mat4 rotZ = glm::rotate(I, glm::radians(-30.0f), glm::vec3(0, 0, 1));
            glm::mat4 trans = glm::translate(I, glm::vec3(0.30f, 0.28f, 0.0f));
            glm::mat4 scale = glm::scale(I, glm::vec3(0.8f, 0.12f, 0.12f));
            glm::mat4 m = parent * rotY * rotZ * trans * scale;
            
            shader.use();
            shader.setVec3("material.ambient", glm::vec3(0.28f, 0.16f, 0.08f));
            shader.setVec3("material.diffuse", glm::vec3(0.32f, 0.18f, 0.09f));
            shader.setVec3("material.specular", glm::vec3(0.14f, 0.11f, 0.07f));
            shader.setFloat("material.shininess", 11.0f);
            shader.setMat4("model", m);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }

    // ------------------------------------------------------------------
    void drawPebbles(Shader& shader, unsigned int vao,
        const glm::mat4& parent, const glm::mat4& I)
    {
        // Create pebbles with better materials and no heavy reliance on texture
        // Use procedural coloring for rock-like appearance
        shader.use();
        
        for (auto& p : pebbles)
        {
            float ar = glm::radians(p.angle);
            float px = p.dist * cosf(ar);
            float pz = p.dist * sinf(ar);

            glm::mat4 trans = glm::translate(I, glm::vec3(px, 0.0f, pz));
            glm::mat4 rotY = glm::rotate(I, glm::radians(p.ry), glm::vec3(0, 1, 0));
            glm::mat4 scale = glm::scale(I, glm::vec3(p.sx, p.sy, p.sz));
            // centre the cube on its position
            glm::mat4 centre = glm::translate(I, glm::vec3(-0.5f, 0.0f, -0.5f));

            glm::mat4 m = parent * trans * rotY * scale * centre;

            // Vary pebble color based on index for realistic stone appearance
            int idx = (&p - pebbles.data()) % pebbles.size();
            float colorVar = 0.3f + 0.7f * (float)(idx % 3) / 3.0f;
            
            // Base grey stone color
            float baseR = 0.45f * colorVar;
            float baseG = 0.42f * colorVar;
            float baseB = 0.40f * colorVar;
            
            // Add slight color variation (reddish, brownish, or darker)
            if (idx % 5 == 0) {
                // Reddish pebble
                baseR += 0.08f;
                baseG -= 0.03f;
            } else if (idx % 5 == 1) {
                // Brownish pebble
                baseR += 0.05f;
                baseG += 0.02f;
                baseB -= 0.02f;
            } else if (idx % 5 == 2) {
                // Darker pebble
                baseR -= 0.05f;
                baseG -= 0.05f;
                baseB -= 0.05f;
            }
            
            shader.setVec3("material.ambient", glm::vec3(baseR * 0.5f, baseG * 0.5f, baseB * 0.5f));
            shader.setVec3("material.diffuse", glm::vec3(baseR, baseG, baseB));
            shader.setVec3("material.specular", glm::vec3(0.25f, 0.24f, 0.22f));
            shader.setFloat("material.shininess", 16.0f);
            shader.setMat4("model", m);
            
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            
            // Add darker spots for realistic weathering
            glm::mat4 spotScale = glm::scale(I, glm::vec3(p.sx * 0.6f, p.sy * 0.5f, p.sz * 0.6f));
            glm::mat4 spotTrans = glm::translate(I, glm::vec3(0.1f, 0.15f, 0.1f));
            glm::mat4 spotM = parent * trans * rotY * spotTrans * spotScale * centre;
            
            shader.setVec3("material.ambient", glm::vec3(baseR * 0.3f - 0.08f, baseG * 0.3f - 0.08f, baseB * 0.3f - 0.08f));
            shader.setVec3("material.diffuse", glm::vec3(baseR - 0.12f, baseG - 0.12f, baseB - 0.12f));
            shader.setVec3("material.specular", glm::vec3(0.15f, 0.14f, 0.12f));
            shader.setFloat("material.shininess", 12.0f);
            shader.setMat4("model", spotM);
            
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
    }

    // ------------------------------------------------------------------
    void drawFire(Shader& shader, unsigned int vao,
        const glm::mat4& parent, const glm::mat4& I,
        float t, float flicker)
    {
        // Fire built from layered, animated flame quads (cubes very flat in Z)
        // Enable blending before drawing fire
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        struct FlameLayer {
            float yOff;    // base Y
            float scaleX;
            float scaleY;
            float phase;   // animation phase offset
            float r, g, b; // colour
        };

        // Layers from base (wide/orange) to tip (narrow/yellow)
        std::vector<FlameLayer> layers = {
            // yOff  sX     sY     phase   r      g      b
            { 0.10f, 0.55f, 0.40f, 0.0f,  0.95f, 0.30f, 0.02f },  // base orange
            { 0.35f, 0.40f, 0.50f, 0.4f,  0.95f, 0.50f, 0.02f },
            { 0.55f, 0.30f, 0.45f, 0.8f,  0.98f, 0.70f, 0.05f },  // mid yellow-orange
            { 0.72f, 0.20f, 0.40f, 1.2f,  1.00f, 0.85f, 0.10f },
            { 0.88f, 0.14f, 0.38f, 1.6f,  1.00f, 0.95f, 0.40f },  // tip yellow
            { 1.00f, 0.08f, 0.28f, 2.0f,  1.00f, 1.00f, 0.70f },  // very tip white
        };

        for (auto& fl : layers)
        {
            // Animate: each layer sways side to side + pulses scale
            float sway = 0.08f * sinf(t * 4.5f + fl.phase);
            float pulse = 1.0f + 0.12f * sinf(t * 6.0f + fl.phase) * flicker;
            float yAnim = fl.yOff + 0.03f * sinf(t * 5.0f + fl.phase * 2.0f);

            glm::mat4 trans = glm::translate(I, glm::vec3(sway, yAnim, sway * 0.5f));
            glm::mat4 scale = glm::scale(I, glm::vec3(fl.scaleX * pulse,
                fl.scaleY * flicker,
                fl.scaleX * pulse));
            glm::mat4 centre = glm::translate(I, glm::vec3(-0.5f, 0.0f, -0.5f));

            glm::mat4 m = parent * trans * scale * centre;
            cf_drawCube(vao, shader, m,
                fl.r * flicker,
                fl.g * flicker,
                fl.b * flicker);
        }

        // Ember sparks: tiny fast cubes that rise and fade
        for (int s = 0; s < 5; s++)
        {
            float sp = (float)s / 5.0f;
            float loop = fmodf(t * (0.6f + sp * 0.3f) + sp, 1.0f); // 0→1 loop
            float sy = 0.1f + loop * 1.6f;
            float sx = 0.08f * sinf(t * 8.0f + sp * 3.14f);
            float sz = 0.08f * cosf(t * 7.0f + sp * 2.0f);
            float fade = 1.0f - loop;               // bright at bottom, fades up
            float es = 0.03f + 0.02f * sp;

            glm::mat4 trans = glm::translate(I, glm::vec3(sx, sy, sz));
            glm::mat4 scale = glm::scale(I, glm::vec3(es, es, es));
            glm::mat4 m = parent * trans * scale;

            cf_drawCube(vao, shader, m,
                1.0f * fade,
                0.55f * fade,
                0.0f);
        }

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
};


