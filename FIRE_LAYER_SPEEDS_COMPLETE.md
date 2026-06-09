# ?? CAMPFIRE FIRE LAYER SPEED - DETAILED BREAKDOWN

---

## QUICK ANSWER TABLE

| Layer | Size | Speed Factor | Base Speed | Effective Speed | Rotation/Sec | Perceptual Effect |
|-------|------|--------------|-----------|-----------------|--------------|-------------------|
| **1** | 1.8×1.8 | 1.0× | 180°/s | **180°/s** | 0.5 | Steady base ?? |
| **2** | 1.6×1.6 | 1.17× | 180°/s | **210.6°/s** | 0.585 | Slow motion ?? |
| **3** | 1.4×1.4 | 1.33× | 180°/s | **239.4°/s** | 0.665 | Medium motion ?? |
| **4** | 1.2×1.2 | 1.5× | 180°/s | **270°/s** | 0.75 | Faster motion ?? |
| **5** | 0.8×0.8 | 1.67× | 180°/s | **300.6°/s** | 0.835 | Very fast ?? |
| **6** | 0.4×0.4 | 1.83× | 180°/s | **329.4°/s** | 0.915 | Rapid flicker ?? |

---

## SPEED CALCULATION FORMULA

```
Speed_Layer[i] = BaseSpeed × SpeedFactor

Where:
  BaseSpeed = 180°/second (constant for all layers)
  SpeedFactor = (7 - LayerIndex) / 6.0
  LayerIndex = 1 to 6 (1 = base, 6 = top)
```

---

## EACH LAYER SPEED - DETAILED

### Layer 1 (Base Layer)

```
Size:               1.8 × 1.8 units
Speed Factor:       (7 - 1) / 6.0 = 6 / 6 = 1.0×
Base Speed:         180° per second
Effective Speed:    180° × 1.0 = 180°/second
Rotations/Second:   180° / 360° = 0.5 rotations/sec
Rotations/Minute:   0.5 × 60 = 30 rotations/minute

Color:              Orange-red (1.0, 0.6, 0.0)
Animation:          Steady, controlled rotation
Visual Effect:      Stable fire base foundation
60 FPS Frame Rate:  180 × 0.01667 = 3.0° per frame
```

---

### Layer 2

```
Size:               1.6 × 1.6 units
Speed Factor:       (7 - 2) / 6.0 = 5 / 6 = 0.833×
Base Speed:         180° per second
Effective Speed:    180° × 0.833 = 149.94°/second ? 150°/second
Rotations/Second:   150° / 360° = 0.417 rotations/sec
Rotations/Minute:   0.417 × 60 = 25 rotations/minute

Color:              Dark orange (1.0, 0.6, 0.0)
Animation:          Slightly faster than layer 1
Visual Effect:      Inner flame base
60 FPS Frame Rate:  150 × 0.01667 = 2.5° per frame
```

---

### Layer 3

```
Size:               1.4 × 1.4 units
Speed Factor:       (7 - 3) / 6.0 = 4 / 6 = 0.667×
Base Speed:         180° per second
Effective Speed:    180° × 0.667 = 120.06°/second ? 120°/second
Rotations/Second:   120° / 360° = 0.333 rotations/sec
Rotations/Minute:   0.333 × 60 = 20 rotations/minute

Color:              Orange (1.0, 0.7, 0.0)
Animation:          Medium-paced rotation
Visual Effect:      Middle flame layer
60 FPS Frame Rate:  120 × 0.01667 = 2.0° per frame
```

---

### Layer 4

```
Size:               1.2 × 1.2 units
Speed Factor:       (7 - 4) / 6.0 = 3 / 6 = 0.5×
Base Speed:         180° per second
Effective Speed:    180° × 0.5 = 90°/second
Rotations/Second:   90° / 360° = 0.25 rotations/sec
Rotations/Minute:   0.25 × 60 = 15 rotations/minute

Color:              Yellow-orange (1.0, 0.8, 0.0)
Animation:          Faster rotation begins
Visual Effect:      Upper-middle flame
60 FPS Frame Rate:  90 × 0.01667 = 1.5° per frame
```

---

### Layer 5

```
Size:               0.8 × 0.8 units
Speed Factor:       (7 - 5) / 6.0 = 2 / 6 = 0.333×
Base Speed:         180° per second
Effective Speed:    180° × 0.333 = 59.94°/second ? 60°/second
Rotations/Second:   60° / 360° = 0.167 rotations/sec
Rotations/Minute:   0.167 × 60 = 10 rotations/minute

Color:              Bright yellow (1.0, 0.9, 0.2)
Animation:          Rapid flickering
Visual Effect:      Upper flame tip
60 FPS Frame Rate:  60 × 0.01667 = 1.0° per frame
```

---

### Layer 6 (Top Layer)

```
Size:               0.4 × 0.4 units
Speed Factor:       (7 - 6) / 6.0 = 1 / 6 = 0.167×
Base Speed:         180° per second
Effective Speed:    180° × 0.167 = 29.97°/second ? 30°/second
Rotations/Second:   30° / 360° = 0.083 rotations/sec
Rotations/Minute:   0.083 × 60 = 5 rotations/minute

Color:              Bright yellow (1.0, 1.0, 0.3)
Animation:          Very rapid flickering
Visual Effect:      Topmost flame (flickering tip)
60 FPS Frame Rate:  30 × 0.01667 = 0.5° per frame
```

---

## COMPARISON VISUALIZATIONS

### Speed Multiplier Hierarchy

```
Layer 1:  [====================] 1.0×   (Slowest)
Layer 2:  [===================>] 0.833×
Layer 3:  [================>] 0.667×
Layer 4:  [=============>] 0.5×
Layer 5:  [=========>] 0.333×
Layer 6:  [=====>] 0.167×                (Fastest)
```

### Rotation Speed Comparison (°/second)

```
Layer 1:  |???????????? 180°/s
Layer 2:  |???????????  150°/s
Layer 3:  |??????????  120°/s
Layer 4:  |????????  90°/s
Layer 5:  |?????  60°/s
Layer 6:  |???  30°/s
```

### Degrees Per Frame at 60 FPS

```
Layer 1:  [3.0°] [3.0°] [3.0°] [3.0°] [3.0°]  = 15° per 5 frames
Layer 2:  [2.5°] [2.5°] [2.5°] [2.5°] [2.5°]  = 12.5° per 5 frames
Layer 3:  [2.0°] [2.0°] [2.0°] [2.0°] [2.0°]  = 10° per 5 frames
Layer 4:  [1.5°] [1.5°] [1.5°] [1.5°] [1.5°]  = 7.5° per 5 frames
Layer 5:  [1.0°] [1.0°] [1.0°] [1.0°] [1.0°]  = 5° per 5 frames
Layer 6:  [0.5°] [0.5°] [0.5°] [0.5°] [0.5°]  = 2.5° per 5 frames
```

---

## TIME-BASED ROTATION PROGRESSION

### At 1 Second (60 FPS = 60 frames)

```
Layer 1:  180° × 1 = 180° (0.5 full rotation)
Layer 2:  150° × 1 = 150° (0.417 rotations)
Layer 3:  120° × 1 = 120° (0.333 rotations)
Layer 4:   90° × 1 = 90°  (0.25 rotations)
Layer 5:   60° × 1 = 60°  (0.167 rotations)
Layer 6:   30° × 1 = 30°  (0.083 rotations)
```

### At 2 Seconds

```
Layer 1:  180° × 2 = 360° (1 full rotation - returns to start)
Layer 2:  150° × 2 = 300° (0.833 rotations)
Layer 3:  120° × 2 = 240° (0.667 rotations)
Layer 4:   90° × 2 = 180° (0.5 rotations)
Layer 5:   60° × 2 = 120° (0.333 rotations)
Layer 6:   30° × 2 = 60°  (0.167 rotations)
```

### At 6 Seconds (All complete full rotations)

```
Layer 1:  180° × 6 = 1080° = 3 full rotations
Layer 2:  150° × 6 = 900° = 2.5 rotations
Layer 3:  120° × 6 = 720° = 2 full rotations
Layer 4:   90° × 6 = 540° = 1.5 rotations
Layer 5:   60° × 6 = 360° = 1 full rotation
Layer 6:   30° × 6 = 180° = 0.5 rotations
```

### At 12 Seconds (All aligned at start again)

```
Layer 1:  180° × 12 = 2160° = 6 full rotations (back to 0°)
Layer 2:  150° × 12 = 1800° = 5 full rotations (back to 0°)
Layer 3:  120° × 12 = 1440° = 4 full rotations (back to 0°)
Layer 4:   90° × 12 = 1080° = 3 full rotations (back to 0°)
Layer 5:   60° × 12 = 720° = 2 full rotations (back to 0°)
Layer 6:   30° × 12 = 360° = 1 full rotation (back to 0°)
```

---

## CODE FORMULA IMPLEMENTATION

```cpp
// In Campfire.h or similar

const float BASE_SPEED = 180.0f;  // Degrees per second

float GetLayerSpeed(int layerIndex) {
    // layerIndex: 1 to 6
    float speedFactor = (7.0f - layerIndex) / 6.0f;
    return BASE_SPEED * speedFactor;
}

// Usage in update loop:
void UpdateFireLayers(float deltaTime) {
    for (int layer = 1; layer <= 6; layer++) {
        float layerSpeed = GetLayerSpeed(layer);
        rotationAngle[layer] += layerSpeed * deltaTime;
        
        if (rotationAngle[layer] >= 360.0f) {
            rotationAngle[layer] -= 360.0f;
        }
    }
}
```

---

## VIVA ANSWER FORMATS

### SHORT ANSWER (30 seconds)
**"Each fire layer rotates at a base speed of 180°/second, but multiplied by a speed factor calculated as (7-layerIndex)/6. Layer 1 (base) rotates at 180°/sec, while Layer 6 (top) rotates at approximately 30°/sec. This creates faster flickering at the top and slower steady base at the bottom."**

### MEDIUM ANSWER (1 minute)
**"The campfire has 6 layers with rotation speeds that vary mathematically. The formula is: Speed = 180 × (7-LayerIndex)/6 degrees per second. Layer 1 at the base rotates at 180°/second (0.5 rotations/sec). Layer 6 at the top rotates at 30°/second (0.083 rotations/sec). The intermediate layers (2-5) have speeds of 150°, 120°, 90°, and 60° per second respectively. This variation creates a realistic flickering fire effect where top layers move rapidly and base layers remain steady."**

### DETAILED ANSWER (Full viva)
**"The fire layer rotation uses a delta-time independent speed calculation. The base angular velocity is 180 degrees per second for all layers. However, each layer's effective speed is modulated by a speed factor: (7 - LayerIndex) / 6.0, where LayerIndex ranges from 1 to 6.

This produces the following speeds:
- Layer 1 (1.0×): 180°/sec
- Layer 2 (0.833×): 150°/sec
- Layer 3 (0.667×): 120°/sec
- Layer 4 (0.5×): 90°/sec
- Layer 5 (0.333×): 60°/sec
- Layer 6 (0.167×): 30°/sec

The velocity update formula in each frame is: angle += speed × deltaTime. At 60 FPS with deltaTime = 0.01667 seconds, Layer 1 rotates 3° per frame while Layer 6 rotates 0.5° per frame. This differential rotation creates the illusion of flickering flames where the top moves rapidly to simulate rising heat and flickering, while the base provides stable structure. All layers rotate independently around the Y-axis (vertical), creating a complex, non-repeating flame animation."**

---

## MEMORY/REFERENCE CARD

```
??????????????????????????????????????????????????????????
?           FIRE LAYER SPEED QUICK REFERENCE             ?
??????????????????????????????????????????????????????????
? BASE SPEED: 180°/second (all layers)                  ?
??????????????????????????????????????????????????????????
? Layer ? Factor  ? Speed    ? Rot/Sec ? 60FPS Frame  ?
?   1   ? 1.0×    ? 180°/s   ? 0.5     ? 3.0°/frame  ?
?   2   ? 0.833×  ? 150°/s   ? 0.417   ? 2.5°/frame  ?
?   3   ? 0.667×  ? 120°/s   ? 0.333   ? 2.0°/frame  ?
?   4   ? 0.5×    ? 90°/s    ? 0.25    ? 1.5°/frame  ?
?   5   ? 0.333×  ? 60°/s    ? 0.167   ? 1.0°/frame  ?
?   6   ? 0.167×  ? 30°/s    ? 0.083   ? 0.5°/frame  ?
??????????????????????????????????????????????????????????
? FORMULA: Speed = 180 × (7-i)/6 degrees per second     ?
? UPDATE:  angle += speed × deltaTime                    ?
??????????????????????????????????????????????????????????
```

---

## VISUAL FIRE EFFECT SUMMARY

```
FASTEST SPINNER (Top)       SLOWEST SPINNER (Bottom)
     ? ? ?                           ? ?
    Layer 6: 30°/s                  Layer 1: 180°/s
    Layer 5: 60°/s                  Layer 2: 150°/s
    Layer 4: 90°/s                  Layer 3: 120°/s
     ? ?                             ? ? ?

Result: Non-repeating flickering motion that looks like
        realistic fire with rising heat and dancing flames
```

---

**END OF FIRE LAYER SPEED DOCUMENTATION**

This reference can be used directly in your viva presentation! ???

