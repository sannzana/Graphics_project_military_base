# JEEP HEADLIGHT SYSTEM - SHORT DESCRIPTION

## Quick Overview

**Jeep Headlights** are procedurally generated spotlights that illuminate the ground/scene ahead of the jeep when turned on.

---

## Lighting Components

### **Headlight Glass (Visual)**
- Located on jeep front bumper (2 sides)
- Changes appearance based on light state:
  - **ON**: Bright yellow-white glass glow
  - **OFF**: Dark/dim appearance

### **Spotlight Beams (Illumination)**
- **4 Total Spotlights** (2 per jeep × 2 jeeps):
  - `jeepAHeadlightL` - Jeep A left
  - `jeepAHeadlightR` - Jeep A right
  - `jeepBHeadlightL` - Jeep B left
  - `jeepBHeadlightR` - Jeep B right

- **Cone Properties**:
  - **Angle**: 20° (narrow, focused beam)
  - **Color**: Bright yellow-white (1.0, 0.95, 0.8)
  - **Position**: On front bumper, 0.6 units left/right apart
  - **Direction**: Always points forward (matches jeep heading)

---

## How It Works

### **Per-Frame Update**
```
1. Get jeep's forward direction (from yawDeg)
2. Calculate headlight positions (front bumper, left & right)
3. Set direction = forward
4. If headlights ON ? bright color
5. If headlights OFF ? no light (0,0,0)
```

### **Light Behavior**
- **Attenuation** (light fades with distance):
  - k_c=1.0, k_l=0.09, k_q=0.032
  - Full intensity at 0m
  - ~50% intensity at 10m
  - ~10% intensity at 25m
  - Barely visible beyond 30m

- **Cone Falloff**:
  - Sharp 20° edge (spotlight cone)
  - Objects outside cone = no lighting
  - Objects inside cone = smooth intensity

---

## Controls

| Key | Action |
|-----|--------|
| **J** | Select/cycle jeeps (None ? Jeep A ? Jeep B ? None) |
| **L** | Toggle selected jeep's headlights ON/OFF |
| **Arrow Keys** | Move/rotate selected jeep |

---

## Shader Implementation

### **Spotlight Calculation** (GLSL)
```glsl
float cos_alpha = dot(L, D);    // Check if fragment in cone
if(cos_alpha > light.cos_theta) {
    intensity = cos_alpha;       // Smooth falloff
} else {
    intensity = 0.0;             // Outside cone
}

// Apply attenuation & intensity to lighting
result *= attenuation * intensity;
```

### **Uniforms per Spotlight**
- `position` - X,Y,Z in world space
- `direction` - Forward direction (normalized)
- `diffuse` - Light color (1.0, 0.95, 0.8)
- `cos_theta` - Cosine of 20° cutoff angle
- `k_c, k_l, k_q` - Attenuation coefficients

---

## Expected Visual Behavior

### **With Headlights ON** ?
- Ground in front of jeep appears **brightly lit**
- Yellow-white cone shaped beam visible
- Beam follows jeep rotation instantly
- Both jeeps can have lights independently

### **With Headlights OFF** ?
- No beam visible
- Scene lit only by ambient/point/directional lights
- Jeep still visible from other light sources

---

## Technical Details

| Property | Value |
|----------|-------|
| Light Type | Spotlight (cone-shaped) |
| Cutoff Angle | 20° |
| Color (ON) | (1.0, 0.95, 0.8) |
| Color (OFF) | (0.0, 0.0, 0.0) |
| Position | Front bumper + 0.6 offset left/right |
| Direction | Matches jeep yaw (forward vector) |
| Shader Passes | 2 (lightingShader + lightingShaderWithTexture) |
| Per-Frame Updates | Yes (position & direction change with jeep) |

---

## Integration Points

1. **JeepState** - Stores `headlightsOn` bool flag
2. **updateJeepHeadlights()** - Updates positions/directions every frame
3. **Fragment Shader** - Applies spotlight calculations to scene
4. **Uniform Arrays** - `spotLights[0..3]` in shader
5. **Key Callback** - L key toggles `headlightsOn`

---

## Debugging

If lights don't show:
1. Check console for "Jeep Selected" message
2. Verify L key toggles headlight state
3. Check shader compilation (no errors?)
4. Verify spotlight parameters are non-zero when ON
5. Ensure camera can see area in front of jeep
