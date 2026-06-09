# JEEP HEADLIGHT DEBUGGING GUIDE

## ? What I Fixed

### 1. **Shader Array Support**
- ? OLD: Only `uniform SpotLight spotLight;` (1 spotlight)
- ? NEW: `uniform SpotLight spotLights[4];` (4 spotlights for both jeeps)

### 2. **Direction Normalization**
- ? OLD: Using `normalize(-light.direction)` (wrong calculation)
- ? NEW: Properly normalizing direction with `D = normalize(light.direction)`

### 3. **Cone Angle Check**
- ? OLD: `if(cos_alpha < light.cos_theta)` was using LESS THAN (backward!)
- ? NEW: `if(cos_alpha > light.cos_theta)` (correct check)

### 4. **Uniform Setup**
- ? OLD: No uniform array setup for multiple spotlights
- ? NEW: Setting all 4 spotlights as array elements [0], [1], [2], [3]

---

## ?? HOW TO TEST

### **Step 1: Select a Jeep**
```
Press J  ?  "None" (initial)
Press J  ?  "Jeep A Selected" (console)
Press J  ?  "Jeep B Selected" (console)
```

### **Step 2: Turn On Headlights**
```
Press L  ?  Headlight glass becomes BRIGHT YELLOW-WHITE
```

### **Step 3: Look at the Ground/Scene**
- Move camera to see the jeep's front
- You should see **bright yellow-white cone of light** in front of the jeep
- Light should illuminate the ground/terrain ahead

### **Step 4: Move the Jeep**
```
Arrow UP/DOWN  ?  Move jeep
Arrow LEFT/RIGHT  ?  Rotate jeep
```
- Light cone should **follow jeep direction**
- Light should **point forward** where jeep is facing

### **Step 5: Test Both Jeeps**
```
Press J  ?  Cycle to Jeep B
Press L  ?  Turn on Jeep B lights
Arrow Keys  ?  Drive around
```
Both jeeps should have independent headlights!

---

## ? IF LIGHTS STILL DON'T SHOW

### Check 1: **Is the direction correct?**
- Print debug info in `updateJeepHeadlights()` function
- Add this to see headlight positions (in console):
```cpp
std::cout << "Jeep " << selectedJeep << " Headlight Pos: (" 
          << headlightBasePosL.x << ", " << headlightBasePosL.y 
          << ", " << headlightBasePosL.z << ")" << std::endl;
```

### Check 2: **Are the shaders compiling?**
- Check shader compile errors
- If shaders fail, spotLights array won't work

### Check 3: **Verify light is ON**
- In `updateJeepHeadlights()`, when `headlightsOn == true`:
  - `diffuse` should be `(1.0, 0.95, 0.8)` (bright yellow-white)
  - Not `(0, 0, 0)` (which is off)

### Check 4: **Distance matters**
- Spotlight cutoff distance:
  - k_c=1.0, k_l=0.09, k_q=0.032
  - At 10 units: attenuation ? 0.7
  - At 20 units: attenuation ? 0.3
  - At 30+ units: very dim

---

## ?? EXPECTED BEHAVIOR

### With Headlights ON:
```
? Jeep model headlight glass = BRIGHT YELLOW
? Forward cone of light visible on ground
? Light follows jeep rotation
? Both jeeps can have independent lights
? Light illuminates trees, terrain, objects ahead
```

### With Headlights OFF:
```
? Jeep model headlight glass = DARK
? No spotlight illumination from jeep
? Only ambient/point/directional lighting applies
```

---

## ?? SPOTLIGHT PARAMETERS REFERENCE

```
Position:      At jeep front, 2.5 units ahead, 0.6 units left/right
Direction:     Forward direction of jeep (normalized)
Cutoff Angle:  20° (narrow cone)
Color:         (1.0, 0.95, 0.8) bright yellow-white when ON
Attenuation:   Realistic falloff with distance
```

---

## ?? MANUAL VERIFICATION COMMANDS

Add to `updateJeepHeadlights()` temporarily:

```cpp
if (jeep.headlightsOn) {
    std::cout << "=== JEEP HEADLIGHT DEBUG ===" << std::endl;
    std::cout << "Jeep Pos: " << jeep.pos.x << ", " << jeep.pos.y << ", " << jeep.pos.z << std::endl;
    std::cout << "Headlight L Pos: " << headlightL.position.x << ", " << headlightL.position.y << ", " << headlightL.position.z << std::endl;
    std::cout << "Headlight Direction: " << headlightL.direction.x << ", " << headlightL.direction.y << ", " << headlightL.direction.z << std::endl;
    std::cout << "Diffuse Color: " << headlightL.diffuse.x << ", " << headlightL.diffuse.y << ", " << headlightL.diffuse.z << std::endl;
    std::cout << "=========================" << std::endl;
}
```

This will print headlight info every frame when on - helps identify positioning issues!
