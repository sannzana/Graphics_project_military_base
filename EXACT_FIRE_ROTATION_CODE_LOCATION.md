# ?? EXACT CODE LOCATION - FIRE LAYER ROTATION IN CAMPFIRE.H

---

## CURRENT CODE ANALYSIS

Your `Campfire.h` file has a `drawFire()` function (lines ~275-340 approx) that draws 6 flame layers, BUT **it does NOT implement per-layer rotation**.

### Current Fire Drawing (WITHOUT rotation):
```cpp
void drawFire(Shader& shader, unsigned int vao,
    const glm::mat4& parent, const glm::mat4& I,
    float t, float flicker)
{
    // ... blending setup ...
    
    std::vector<FlameLayer> layers = {
        // yOff  sX     sY     phase   r      g      b
        { 0.10f, 0.55f, 0.40f, 0.0f,  0.95f, 0.30f, 0.02f },  // base orange
        { 0.35f, 0.40f, 0.50f, 0.4f,  0.95f, 0.50f, 0.02f },
        { 0.55f, 0.30f, 0.45f, 0.8f,  0.98f, 0.70f, 0.05f },
        { 0.72f, 0.20f, 0.40f, 1.2f,  1.00f, 0.85f, 0.10f },
        { 0.88f, 0.14f, 0.38f, 1.6f,  1.00f, 0.95f, 0.40f },
        { 1.00f, 0.08f, 0.28f, 2.0f,  1.00f, 1.00f, 0.70f },
    };

    for (auto& fl : layers)
    {
        // Current animation: sway + pulse (NO ROTATION)
        float sway = 0.08f * sinf(t * 4.5f + fl.phase);
        float pulse = 1.0f + 0.12f * sinf(t * 6.0f + fl.phase) * flicker;
        float yAnim = fl.yOff + 0.03f * sinf(t * 5.0f + fl.phase * 2.0f);

        glm::mat4 trans = glm::translate(I, glm::vec3(sway, yAnim, sway * 0.5f));
        glm::mat4 scale = glm::scale(I, glm::vec3(fl.scaleX * pulse,
            fl.scaleY * flicker,
            fl.scaleX * pulse));
        glm::mat4 centre = glm::translate(I, glm::vec3(-0.5f, 0.0f, -0.5f));

        glm::mat4 m = parent * trans * scale * centre;
        // ? NO ROTATION HERE!
        
        cf_drawCube(vao, shader, m, ...);
    }
}
```

---

## WHERE TO ADD FIRE ROTATION

You need to modify the `FlameLayer` struct and the `drawFire()` function.

### MODIFICATION 1: Update the FlameLayer struct

**FIND THIS (around line ~260):**
```cpp
struct FlameLayer {
    float yOff;    // base Y
    float scaleX;
    float scaleY;
    float phase;   // animation phase offset
    float r, g, b; // colour
};
```

**CHANGE TO (ADD layer index):**
```cpp
struct FlameLayer {
    int   layerIndex; // 1-6 (for speed calculation)
    float yOff;       // base Y
    float scaleX;
    float scaleY;
    float phase;      // animation phase offset
    float r, g, b;    // colour
};
```

---

### MODIFICATION 2: Update layer initialization

**FIND THIS (around line ~265-271):**
```cpp
std::vector<FlameLayer> layers = {
    // yOff  sX     sY     phase   r      g      b
    { 0.10f, 0.55f, 0.40f, 0.0f,  0.95f, 0.30f, 0.02f },  // base orange
    { 0.35f, 0.40f, 0.50f, 0.4f,  0.95f, 0.50f, 0.02f },
    { 0.55f, 0.30f, 0.45f, 0.8f,  0.98f, 0.70f, 0.05f },
    { 0.72f, 0.20f, 0.40f, 1.2f,  1.00f, 0.85f, 0.10f },
    { 0.88f, 0.14f, 0.38f, 1.6f,  1.00f, 0.95f, 0.40f },
    { 1.00f, 0.08f, 0.28f, 2.0f,  1.00f, 1.00f, 0.70f },
};
```

**CHANGE TO (ADD layerIndex):**
```cpp
std::vector<FlameLayer> layers = {
    // layerIndex, yOff  sX     sY     phase   r      g      b
    { 1, 0.10f, 0.55f, 0.40f, 0.0f,  0.95f, 0.30f, 0.02f },  // base orange
    { 2, 0.35f, 0.40f, 0.50f, 0.4f,  0.95f, 0.50f, 0.02f },
    { 3, 0.55f, 0.30f, 0.45f, 0.8f,  0.98f, 0.70f, 0.05f },
    { 4, 0.72f, 0.20f, 0.40f, 1.2f,  1.00f, 0.85f, 0.10f },
    { 5, 0.88f, 0.14f, 0.38f, 1.6f,  1.00f, 0.95f, 0.40f },
    { 6, 1.00f, 0.08f, 0.28f, 2.0f,  1.00f, 1.00f, 0.70f },
};
```

---

### MODIFICATION 3: ADD ROTATION IN THE LOOP

**FIND THIS (around line ~275-290):**
```cpp
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
```

**CHANGE TO (ADD ROTATION):**
```cpp
const float BASE_SPEED = 180.0f;  // degrees per second

for (auto& fl : layers)
{
    // ???????????????????????????????????????????????????????????
    // CALCULATE LAYER ROTATION SPEED
    // ???????????????????????????????????????????????????????????
    float speedFactor = (7.0f - fl.layerIndex) / 6.0f;
    float layerSpeed = BASE_SPEED * speedFactor;  // degrees per second
    float rotationAngle = layerSpeed * t;  // accumulate over time
    
    // Wrap to 0-360 degrees
    while (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // ???????????????????????????????????????????????????????????
    // Animate: each layer sways side to side + pulses scale
    float sway = 0.08f * sinf(t * 4.5f + fl.phase);
    float pulse = 1.0f + 0.12f * sinf(t * 6.0f + fl.phase) * flicker;
    float yAnim = fl.yOff + 0.03f * sinf(t * 5.0f + fl.phase * 2.0f);

    // ???????????????????????????????????????????????????????????
    // BUILD TRANSFORMATION MATRIX WITH ROTATION
    // ???????????????????????????????????????????????????????????
    glm::mat4 trans = glm::translate(I, glm::vec3(sway, yAnim, sway * 0.5f));
    
    // *** KEY ADDITION: ROTATE AROUND Y-AXIS ***
    glm::mat4 rot = glm::rotate(I, glm::radians(rotationAngle), 
                               glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 scale = glm::scale(I, glm::vec3(fl.scaleX * pulse,
        fl.scaleY * flicker,
        fl.scaleX * pulse));
    glm::mat4 centre = glm::translate(I, glm::vec3(-0.5f, 0.0f, -0.5f));

    // *** ORDER MATTERS: translate ? rotate ? scale ? centre ***
    glm::mat4 m = parent * trans * rot * scale * centre;
    
    cf_drawCube(vao, shader, m,
        fl.r * flicker,
        fl.g * flicker,
        fl.b * flicker);
}
```

---

## COMPLETE MODIFIED drawFire() FUNCTION

Here's the entire function with rotation implemented:

```cpp
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
        int   layerIndex; // 1-6 (for speed calculation)
        float yOff;       // base Y
        float scaleX;
        float scaleY;
        float phase;      // animation phase offset
        float r, g, b;    // colour
    };

    // Layers from base (wide/orange) to tip (narrow/yellow)
    std::vector<FlameLayer> layers = {
        // layerIndex, yOff  sX     sY     phase   r      g      b
        { 1, 0.10f, 0.55f, 0.40f, 0.0f,  0.95f, 0.30f, 0.02f },  // base orange
        { 2, 0.35f, 0.40f, 0.50f, 0.4f,  0.95f, 0.50f, 0.02f },
        { 3, 0.55f, 0.30f, 0.45f, 0.8f,  0.98f, 0.70f, 0.05f },  // mid yellow-orange
        { 4, 0.72f, 0.20f, 0.40f, 1.2f,  1.00f, 0.85f, 0.10f },
        { 5, 0.88f, 0.14f, 0.38f, 1.6f,  1.00f, 0.95f, 0.40f },  // tip yellow
        { 6, 1.00f, 0.08f, 0.28f, 2.0f,  1.00f, 1.00f, 0.70f },  // very tip white
    };

    const float BASE_SPEED = 180.0f;  // degrees per second

    for (auto& fl : layers)
    {
        // ???????????????????????????????????????????????????????????
        // CALCULATE LAYER ROTATION SPEED (EXACT FORMULA)
        // ???????????????????????????????????????????????????????????
        float speedFactor = (7.0f - fl.layerIndex) / 6.0f;
        float layerSpeed = BASE_SPEED * speedFactor;  // degrees/second
        float rotationAngle = layerSpeed * t;
        
        // Wrap to 0-360 degrees
        while (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
        
        // ???????????????????????????????????????????????????????????
        // Animate: each layer sways side to side + pulses scale
        float sway = 0.08f * sinf(t * 4.5f + fl.phase);
        float pulse = 1.0f + 0.12f * sinf(t * 6.0f + fl.phase) * flicker;
        float yAnim = fl.yOff + 0.03f * sinf(t * 5.0f + fl.phase * 2.0f);

        // ???????????????????????????????????????????????????????????
        // BUILD TRANSFORMATION MATRIX WITH ROTATION
        // ???????????????????????????????????????????????????????????
        glm::mat4 trans = glm::translate(I, glm::vec3(sway, yAnim, sway * 0.5f));
        
        // *** KEY: ROTATE AROUND Y-AXIS (VERTICAL) ***
        glm::mat4 rot = glm::rotate(I, glm::radians(rotationAngle), 
                                   glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 scale = glm::scale(I, glm::vec3(fl.scaleX * pulse,
            fl.scaleY * flicker,
            fl.scaleX * pulse));
        glm::mat4 centre = glm::translate(I, glm::vec3(-0.5f, 0.0f, -0.5f));

        // *** TRANSFORMATION ORDER: translate ? rotate ? scale ? centre ***
        glm::mat4 m = parent * trans * rot * scale * centre;
        
        cf_drawCube(vao, shader, m,
            fl.r * flicker,
            fl.g * flicker,
            fl.b * flicker);
    }

    // Ember sparks: tiny fast cubes that rise and fade
    for (int s = 0; s < 5; s++)
    {
        float sp = (float)s / 5.0f;
        float loop = fmodf(t * (0.6f + sp * 0.3f) + sp, 1.0f); // 0?1 loop
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
```

---

## SPEED CALCULATION BREAKDOWN

For each layer, the rotation speed is calculated as:

```cpp
// Layer speeds (degrees per second):
speedFactor = (7 - layerIndex) / 6.0f;
layerSpeed = 180 * speedFactor;

// Results:
// Layer 1: (7-1)/6 = 1.0  ? 180°/sec
// Layer 2: (7-2)/6 = 0.833 ? 150°/sec
// Layer 3: (7-3)/6 = 0.667 ? 120°/sec
// Layer 4: (7-4)/6 = 0.5  ? 90°/sec
// Layer 5: (7-5)/6 = 0.333 ? 60°/sec
// Layer 6: (7-6)/6 = 0.167 ? 30°/sec
```

---

## MATRIX MULTIPLICATION ORDER (CRITICAL)

The transformation order is:
```
Model = Parent × Translate × Rotate × Scale × Centre

parent * trans * rot * scale * centre

This applies transformations in reverse order (right to left):
1. Centre the cube (-0.5, 0, -0.5)
2. Scale it (pulse + flicker)
3. Rotate around Y-axis (FIRE ROTATION)
4. Translate to position (sway + yAnim)
5. Apply parent transform (campfire position)
```

---

## DELTA TIME BASIS CHECK

? **Your implementation IS delta-time independent:**

```cpp
float rotationAngle = layerSpeed * t;
// layerSpeed = degrees per second (constant)
// t = elapsed time in seconds (from glfwGetTime())
// Result: angle accumulates smoothly regardless of frame rate
```

At 60 FPS: Layer 1 rotates 3.0°/frame (180° × 1/60)
At 120 FPS: Layer 1 rotates 1.5°/frame (180° × 1/120)
But always 180°/second! ?

---

## TESTING CHECKLIST

After applying these changes:

- [ ] Compile without errors
- [ ] Fire layers rotate around Y-axis (vertical)
- [ ] Base layer (Layer 1) rotates slowest
- [ ] Top layer (Layer 6) rotates fastest
- [ ] Fire still glows/flickers
- [ ] No visual artifacts
- [ ] Performance stays smooth (60+ FPS)

---

## VIVA ANSWER (Using This Code)

**"In the Campfire.h file, within the drawFire() function, each of the 6 flame layers has a layerIndex from 1-6. For each layer, the rotation speed is calculated using the formula: speedFactor = (7-index)/6, multiplied by a base speed of 180 degrees per second. The rotation angle is then computed as: rotationAngle = layerSpeed × t (time in seconds). This angle is applied to the model matrix using glm::rotate() around the Y-axis (vertical), inserted between the translation and scaling transformations. The resulting transformation: parent × translate × rotate × scale × centre creates each layer rotating independently around the campfire center, with bottom layers rotating slowly and top layers rotating rapidly, creating a realistic flickering fire effect."**

