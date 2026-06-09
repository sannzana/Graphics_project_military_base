#include "Barrack.h"

float barrackDoorAngle = 0.0f;
bool barrackDoorOpening = false;

static void drawColorCube(unsigned int& cubeVAO, Shader& lightingShader,
    const glm::mat4& model,
    float r, float g, float b)
{
    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setVec3("material.ambient", glm::vec3(r * 0.35f, g * 0.35f, b * 0.35f));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setFloat("material.shininess", 16.0f);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void updateBarrackDoor()
{
    float speed = 1.2f;

    if (barrackDoorOpening)
    {
        barrackDoorAngle += speed;
        if (barrackDoorAngle > 105.0f) barrackDoorAngle = 105.0f;
    }
    else
    {
        barrackDoorAngle -= speed;
        if (barrackDoorAngle < 0.0f) barrackDoorAngle = 0.0f;
    }
}

static void drawBunkBed(unsigned int& cubeVAO,
    Shader& lightingShader,
    Shader& lightingShaderWithTexture,
    const glm::mat4& parentModel,
    Cube& woodCube)
{
    glm::mat4 I(1.0f), T, S, model;
    float scale = 1.3f;  // 30% bigger beds
    float mattressScale = 1.5f;  // mattresses are 50% bigger

    // lower frame - adjusted to fit larger mattress
    T = glm::translate(I, glm::vec3(0.0f, 0.45f * scale, 0.0f));
    S = glm::scale(I, glm::vec3(2.3f * scale * mattressScale, 0.18f * scale, 0.95f * scale * mattressScale));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // upper frame - adjusted to fit larger mattress
    T = glm::translate(I, glm::vec3(0.0f, 1.85f * scale, 0.0f));
    S = glm::scale(I, glm::vec3(2.3f * scale * mattressScale, 0.18f * scale, 0.95f * scale * mattressScale));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // posts - adjusted for larger bed
    float px[] = { 0.0f, 2.15f, 0.0f, 2.15f };
    float pz[] = { 0.0f, 0.0f, 0.8f, 0.8f };

    for (int i = 0; i < 4; i++)
    {
        T = glm::translate(I, glm::vec3(px[i] * scale * mattressScale, 0.0f, pz[i] * scale * mattressScale));
        S = glm::scale(I, glm::vec3(0.12f * scale, 2.1f * scale, 0.12f * scale));
        model = parentModel * T * S;
        woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    // mattresses - larger sleeping area (50% bigger)
    T = glm::translate(I, glm::vec3(0.08f * scale, 0.62f * scale, 0.06f * scale));
    S = glm::scale(I, glm::vec3(2.08f * scale * mattressScale, 0.16f * scale * 1.3f, 0.82f * scale * mattressScale));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.20f, 0.28f, 0.20f);

    T = glm::translate(I, glm::vec3(0.08f * scale, 2.02f * scale, 0.06f * scale));
    S = glm::scale(I, glm::vec3(2.08f * scale * mattressScale, 0.16f * scale * 1.3f, 0.82f * scale * mattressScale));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.20f, 0.28f, 0.20f);

    // pillows - adjusted for larger bed
    T = glm::translate(I, glm::vec3(1.62f * scale * mattressScale, 0.78f * scale, 0.16f * scale * mattressScale));
    S = glm::scale(I, glm::vec3(0.38f * scale * mattressScale, 0.10f * scale, 0.28f * scale * mattressScale));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.92f, 0.92f, 0.92f);

    T = glm::translate(I, glm::vec3(1.62f * scale * mattressScale, 2.18f * scale, 0.16f * scale * mattressScale));
    S = glm::scale(I, glm::vec3(0.38f * scale * mattressScale, 0.10f * scale, 0.28f * scale * mattressScale));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.92f, 0.92f, 0.92f);
}

void drawBarrack(
    unsigned int& cubeVAO,
    Shader& lightingShader,
    Shader& lightingShaderWithTexture,
    const glm::mat4& parentModel,
    Cube& woodCube,
    Cube& roofCube
)
{
    glm::mat4 I(1.0f), T, R, S, model;

    // =========================
    // MAIN BUILDING SIZE
    // =========================
    float W = 18.0f;
    float H = 5.0f;
    float D = 28.0f;
    float t = 0.25f;

    // =========================
    // DOOR
    // =========================
    float doorW = 2.4f;
    float doorH = 4.0f;
    float sideFrontW = (W - doorW) / 2.0f;

    // =========================
    // ROOF CONTROL
    // =========================
    float roofAngle = 28.0f;       // roof slope angle
    float roofThickness = 0.22f;   // slab thickness
    float roofLength = 10.2f;      // each side length
    float ridgeY = H + 4.75f;      // top meeting height
    float overhang = 0.45f;        // outside overhang

    // =========================
    // SUPPORT CONTROL
    // =========================
    float ridgeBeamY = H + 2.30f;     // inner horizontal ridge beam
    float postWidth = 0.16f;
    float braceThickness = 0.12f;
    float braceDepth = 0.16f;

    // =========================
    // FLOOR
    // =========================
    T = glm::translate(I, glm::vec3(-W / 2.0f, 0.0f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(W, 0.18f, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // BACK WALL
    // =========================
    T = glm::translate(I, glm::vec3(-W / 2.0f, 0.0f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(W, H, t));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // LEFT WALL
    // =========================
    T = glm::translate(I, glm::vec3(-W / 2.0f, 0.0f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(t, H, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // RIGHT WALL
    // =========================
    T = glm::translate(I, glm::vec3(W / 2.0f - t, 0.0f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(t, H, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // FRONT WALL WITH DOOR GAP
    // =========================
    T = glm::translate(I, glm::vec3(-W / 2.0f, 0.0f, D / 2.0f - t));
    S = glm::scale(I, glm::vec3(sideFrontW, H, t));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    T = glm::translate(I, glm::vec3(doorW / 2.0f, 0.0f, D / 2.0f - t));
    S = glm::scale(I, glm::vec3(sideFrontW, H, t));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    T = glm::translate(I, glm::vec3(-doorW / 2.0f, doorH, D / 2.0f - t));
    S = glm::scale(I, glm::vec3(doorW, H - doorH, t));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // TOP EDGE BANDS
    // =========================
    T = glm::translate(I, glm::vec3(-W / 2.0f, H - 0.08f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(W, 0.12f, t));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    T = glm::translate(I, glm::vec3(-W / 2.0f, H - 0.08f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(t, 0.12f, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    T = glm::translate(I, glm::vec3(W / 2.0f - t, H - 0.08f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(t, 0.12f, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // DOOR
    // =========================
    glm::mat4 hinge =
        parentModel *
        glm::translate(I, glm::vec3(-doorW / 2.0f, 0.0f, D / 2.0f - t - 0.01f));

    model =
        hinge *
        glm::translate(I, glm::vec3(0.0f, 0.0f, t * 0.5f)) *
        glm::rotate(I, glm::radians(-barrackDoorAngle), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::scale(I, glm::vec3(doorW, doorH, t));

    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // DOOR STEP
    // =========================
    T = glm::translate(I, glm::vec3(-1.4f, 0.0f, D / 2.0f + 0.8f));
    S = glm::scale(I, glm::vec3(2.8f, 0.25f, 1.3f));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // WINDOWS
    // =========================
    float wz[] = { -10.0f, -3.0f, 4.0f, 11.0f };

    for (int i = 0; i < 4; i++)
    {
        T = glm::translate(I, glm::vec3(-W / 2.0f - 0.01f, 2.05f, wz[i]));
        S = glm::scale(I, glm::vec3(0.10f, 1.65f, 1.55f));
        model = parentModel * T * S;
        drawColorCube(cubeVAO, lightingShader, model, 0.75f, 0.82f, 0.88f);

        T = glm::translate(I, glm::vec3(W / 2.0f - 0.09f, 2.05f, wz[i]));
        S = glm::scale(I, glm::vec3(0.10f, 1.65f, 1.55f));
        model = parentModel * T * S;
        drawColorCube(cubeVAO, lightingShader, model, 0.75f, 0.82f, 0.88f);
    }

    // =========================
    // ROOF LEFT HALF
    // =========================
    model =
        parentModel *
        glm::translate(I, glm::vec3(0.0f, ridgeY, -D / 2.0f)) *
        glm::rotate(I, glm::radians(roofAngle), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::translate(I, glm::vec3(-roofLength + overhang, -roofThickness * 0.5f, 0.0f)) *
        glm::scale(I, glm::vec3(roofLength, roofThickness, D));

    roofCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // ROOF RIGHT HALF
    // =========================
    model =
        parentModel *
        glm::translate(I, glm::vec3(0.0f, ridgeY, -D / 2.0f)) *
        glm::rotate(I, glm::radians(-roofAngle), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::translate(I, glm::vec3(-overhang, -roofThickness * 0.5f, 0.0f)) *
        glm::scale(I, glm::vec3(roofLength, roofThickness, D));

    roofCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // FRONT GABLE FILL (stacked planks forming triangle)
    // =========================
    {
        float gableH = ridgeY - H;
        int layers = 14;
        float plankH = gableH / layers;

        for (int i = 0; i < layers; i++)
        {
            float y = H + i * plankH;
            float ratio = 1.0f - (float)i / layers;   // wide at bottom, narrow at top
            float curW = W * ratio;

            T = glm::translate(I, glm::vec3(-curW * 0.5f, y, D / 2.0f - t));
            S = glm::scale(I, glm::vec3(curW, plankH + 0.02f, t));
            model = parentModel * T * S;
            woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);
        }
    }

    // =========================
    // BACK GABLE FILL (stacked planks forming triangle)
    // =========================
    {
        float gableH = ridgeY - H;
        int layers = 14;
        float plankH = gableH / layers;

        for (int i = 0; i < layers; i++)
        {
            float y = H + i * plankH;
            float ratio = 1.0f - (float)i / layers;   // wide at bottom, narrow at top
            float curW = W * ratio;

            T = glm::translate(I, glm::vec3(-curW * 0.5f, y, -D / 2.0f));
            S = glm::scale(I, glm::vec3(curW, plankH + 0.02f, t));
            model = parentModel * T * S;
            woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);
        }
    }

    // =========================
    // RIDGE CAP
    // =========================
    T = glm::translate(I, glm::vec3(-0.14f, ridgeY + 0.02f, -D / 2.0f));
    S = glm::scale(I, glm::vec3(0.28f, 0.16f, D));
    model = parentModel * T * S;
    roofCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // INNER RIDGE BEAM
    // =========================
    T = glm::translate(I, glm::vec3(-0.10f, ridgeBeamY, -D / 2.0f));
    S = glm::scale(I, glm::vec3(0.20f, 0.20f, D));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // INNER SUPPORTS - ALIGNED WITH ROOF (ANGLED BEAMS)
    // =========================
    // These beams slope from the walls up to meet/touch the roof
    // They provide realistic structural support just like a real building
    
    float beamZ[] = { -11.0f, -5.5f, 0.0f, 5.5f, 11.0f };
    
    // Beam properties tuned to match roof slope and touch it
    float roofTouchY = ridgeY - 0.15f;  // slightly below ridge to touch roof underside
    float beamStartX_left = -W / 2.0f + 0.15f;   // starting from left wall (inside)
    float beamStartX_right = W / 2.0f - 0.15f;   // starting from right wall (inside)
    float beamStartY = H + 0.10f;                // starts just above wall top
    
    // Calculate beam length and angle to match roof
    float beamHeightDiff = roofTouchY - beamStartY;  // vertical distance
    float beamHorizontalDist = (W / 2.0f) - 0.5f;   // horizontal distance to center
    float beamLength = sqrtf(beamHeightDiff * beamHeightDiff + beamHorizontalDist * beamHorizontalDist);
    
    for (int i = 0; i < 5; i++)
    {
        // === CENTER VERTICAL POST ===
        // Main support pole reaching toward ridge
        T = glm::translate(I, glm::vec3(-0.08f, H, beamZ[i]));
        S = glm::scale(I, glm::vec3(0.16f, beamHeightDiff + 0.20f, 0.16f));
        model = parentModel * T * S;
        woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);
       
        
        // === HORIZONTAL COLLAR BEAM (ties the structure together) ===
        // Runs left to right at mid-height, connects the support structure
        T = glm::translate(I, glm::vec3(-W / 2.0f + 0.50f, H + beamHeightDiff * 0.45f, beamZ[i]));
        S = glm::scale(I, glm::vec3(W - 1.0f, 0.18f, 0.18f));
        model = parentModel * T * S;
        woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);
    }

    // =========================
    // BEDS
    // =========================
    float bedZ[] = { -10.5f, -5.0f, 0.5f, 6.0f };
    for (int i = 0; i < 4; i++)
    {
        T = glm::translate(I, glm::vec3(-W / 2.0f + 1.0f, 0.0f, bedZ[i]));
        R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = parentModel * T * R;
        drawBunkBed(cubeVAO, lightingShader, lightingShaderWithTexture, model, woodCube);

        T = glm::translate(I, glm::vec3(W / 2.0f - 1.0f, 0.0f, bedZ[i] + 0.8f));
        R = glm::rotate(I, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = parentModel * T * R;
        drawBunkBed(cubeVAO, lightingShader, lightingShaderWithTexture, model, woodCube);
    }

    // =========================
    // CENTER CHEST / TABLE
    // =========================
    T = glm::translate(I, glm::vec3(-1.5f, 0.35f, -1.0f));
    S = glm::scale(I, glm::vec3(3.0f, 0.9f, 1.3f));
    model = parentModel * T * S;
    woodCube.drawCubeWithTexture(lightingShaderWithTexture, model);

    // =========================
    // INTERIOR LIGHTING - MILITARY STYLE TUBE LIGHTS
    // =========================
    // Three long fluorescent tube lights running along the ceiling
    // These create realistic directional interior lighting
    
    // === TUBE LIGHT 1 (Left side) ===
    // Long tube fixture near left side of ceiling
    T = glm::translate(I, glm::vec3(-5.0f, H - 0.35f, -D / 2.0f + 0.5f));
    S = glm::scale(I, glm::vec3(0.10f, 0.14f, D - 2.0f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.20f, 0.20f, 0.18f);  // dark casing
    
    // Tube light glow area (visual element) - MUCH BRIGHTER
    T = glm::translate(I, glm::vec3(-5.0f, H - 0.32f, -D / 2.0f + 0.5f));
    S = glm::scale(I, glm::vec3(0.06f, 0.10f, D - 2.0f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.95f);  // maximum brightness white
    
    // === TUBE LIGHT 2 (Right side) ===
    // Long tube fixture near right side of ceiling
    T = glm::translate(I, glm::vec3(5.0f, H - 0.35f, -D / 2.0f + 0.5f));
    S = glm::scale(I, glm::vec3(0.10f, 0.14f, D - 2.0f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.20f, 0.20f, 0.18f);  // dark casing
    
    // Tube light glow area (visual element) - MUCH BRIGHTER
    T = glm::translate(I, glm::vec3(5.0f, H - 0.32f, -D / 2.0f + 0.5f));
    S = glm::scale(I, glm::vec3(0.06f, 0.10f, D - 2.0f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.95f);  // maximum brightness white
    
    // === CENTER TUBE LIGHT (Running down the middle) ===
    // Main illumination tube - LARGER AND BRIGHTER
    T = glm::translate(I, glm::vec3(0.0f, H - 0.32f, -D / 2.0f + 0.5f));
    S = glm::scale(I, glm::vec3(0.08f, 0.12f, D - 2.0f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 0.98f);  // maximum brightness white

    // =========================
    // FRONT LAMP SHADE
    // =========================
    T = glm::translate(I, glm::vec3(-0.4f, H + 1.6f, D / 2.0f + 0.15f));
    S = glm::scale(I, glm::vec3(0.8f, 0.22f, 0.8f));
    model = parentModel * T * S;
    drawColorCube(cubeVAO, lightingShader, model, 0.12f, 0.12f, 0.12f);
}