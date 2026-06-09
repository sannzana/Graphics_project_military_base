# Jeep Headlight Spotlights Implementation

## Overview
Added dynamic spotlight functionality for jeep headlights that can be toggled on/off with the **L** key when a jeep is selected.

## Changes Made

### 1. **JeepState Structure** (Already had `headlightsOn` member)
- Uses existing `bool headlightsOn` flag to track light state
- Updated when pressing **L** key

### 2. **Four New Spotlight Objects Added**
Created spotlights for each jeep's two headlights:
```cpp
SpotLight jeepAHeadlightL  // Jeep A - Left headlight
SpotLight jeepAHeadlightR  // Jeep A - Right headlight
SpotLight jeepBHeadlightL  // Jeep B - Left headlight
SpotLight jeepBHeadlightR  // Jeep B - Right headlight
```

**Spotlight Properties:**
- **Cutoff Angle:** 20° (narrow cone for focused headlight beam)
- **Color:** Bright yellow-white when ON: `(1.0, 0.95, 0.8)`
- **Attenuation:** k_c=1.0, k_l=0.09, k_q=0.032
- **Turn ON/OFF:** Diffuse and specular set to (0,0,0) when lights are off

### 3. **New Helper Function: `updateJeepHeadlights()`**
```cpp
void updateJeepHeadlights(JeepState& jeep, SpotLight& headlightL, SpotLight& headlightR)
```

**Functionality:**
- Calculates headlight positions based on jeep position, rotation, and direction
- Positions lights **2.5 units** in front of jeep at headlight height
- Left/Right lights positioned **0.6 units** to each side
- Sets spotlight direction to match jeep's forward direction
- Updates light colors based on `headlights_on` flag

**Position Calculation:**
```
Forward direction = jeepForward(jeepYaw)
Right direction = cross(forward, upVector)

HeadlightL_Pos = JeepPos + Forward*2.5 - Right*0.6 + Height
HeadlightR_Pos = JeepPos + Forward*2.5 + Right*0.6 + Height
```

### 4. **Render Loop Integration**

**Update Phase (per-frame):**
```cpp
// After updating jeep animations
updateJeepHeadlights(jeepA, jeepAHeadlightL, jeepAHeadlightR);
updateJeepHeadlights(jeepB, jeepBHeadlightL, jeepBHeadlightR);
```

**Rendering Phase (two shaders):**
- Added to `lightingShader` configuration:
  ```cpp
  jeepAHeadlightL.setUpSpotLight(lightingShader);
  jeepAHeadlightR.setUpSpotLight(lightingShader);
  jeepBHeadlightL.setUpSpotLight(lightingShader);
  jeepBHeadlightR.setUpSpotLight(lightingShader);
  ```

- Added to `lightingShaderWithTexture` configuration:
  ```cpp
  jeepAHeadlightL.setUpSpotLight(lightingShaderWithTexture);
  jeepAHeadlightR.setUpSpotLight(lightingShaderWithTexture);
  jeepBHeadlightL.setUpSpotLight(lightingShaderWithTexture);
  jeepBHeadlightR.setUpSpotLight(lightingShaderWithTexture);
  ```

## User Interaction

### Toggling Jeep Headlights
1. **Select a jeep:**
   - Press **J** to cycle: None ? Jeep A ? Jeep B ? None

2. **Toggle lights ON/OFF:**
   - Press **L** while jeep is selected
   - Headlights visually activate on jeep model
   - Spotlight beams illuminate the scene

### Visual Feedback
- **Jeep Model:** Headlight glass becomes bright yellow-white when on
- **Scene Lighting:** Spotlights cast illumination in front of jeep showing directional light effect
- **Movement:** Headlight positions and directions update as jeep moves and rotates

## Technical Details

### Spotlight Attenuation Formula
```
Attenuation = 1.0 / (k_c + k_l*distance + k_q*distance²)
```
Current values provide good falloff range for a driving light.

### Cutoff Angle
- 20° total angle creates a focused beam typical of vehicle headlights
- Can be adjusted by modifying the SpotLight constructor parameter

### Direction Update
- Headlights always point in the direction the jeep is facing
- Direction = `jeepForward(jeep.yawDeg)` = normalized vector based on yaw rotation

## Future Improvements
- Add visual brake lights (red spotlights at rear)
- Add turn signals (amber spotlights on sides)
- Add high-beam/low-beam toggle
- Add headlight damage effects
- Adjust light intensity based on vehicle speed
