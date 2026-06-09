# MILITARY BASE CAMP - COMPLETE LIGHTING SYSTEM DOCUMENTATION

## Table of Contents
1. [Lighting Theory](#lighting-theory)
2. [Light Types Used](#light-types-used)
3. [All Lights in Scene](#all-lights-in-scene)
4. [Key Controls](#key-controls)
5. [Placement Coordinates](#placement-coordinates)
6. [Technical Implementation](#technical-implementation)

---

## LIGHTING THEORY

### What is Lighting in Computer Graphics?

Lighting simulation in 3D graphics calculates how light interacts with surfaces to create realistic visual appearance. It determines the color and intensity of each pixel based on light sources and material properties.

### Three Components of Lighting Model (Phong Model)

#### 1. **Ambient Light**
- **Definition**: Global illumination that lights all surfaces equally
- **Purpose**: Provides baseline visibility; simulates light bouncing around the scene
- **Characteristics**:
  - No direction
  - Same intensity everywhere
  - Prevents complete darkness in shadow areas
  - Computationally cheap
- **Formula**: `Ambient = Ambient_Light_Color × Material_Ambient_Color`
- **Example in Scene**: Base 5% ambient on all point lights creates subtle background illumination

#### 2. **Diffuse Light**
- **Definition**: Light that scatters in all directions when hitting a surface
- **Purpose**: Gives surfaces their base color and creates basic shading
- **Characteristics**:
  - Depends on surface normal and light direction
  - Follows Lambert's Cosine Law: intensity ? cos(angle between normal and light)
  - Matte surfaces (like grass, walls) reflect diffuse light
  - No dependency on viewer position
- **Formula**: `Diffuse = Diffuse_Light_Color × Material_Diffuse_Color × max(0, Normal · LightDir)`
- **Example in Scene**: 80% diffuse on point lights illuminates ground and buildings

#### 3. **Specular Light**
- **Definition**: Mirror-like reflections from shiny surfaces
- **Purpose**: Creates highlights and shows surface shininess/reflectivity
- **Characteristics**:
  - Depends on light direction, surface normal, AND viewer position
  - Sharp, focused highlights on glossy surfaces
  - Follows Phong reflection model
  - Controlled by shininess value (higher = sharper highlights)
- **Formula**: `Specular = Specular_Light_Color × Material_Specular_Color × pow(max(0, Reflect · ViewDir), Shininess)`
- **Example in Scene**: Shiny metal objects (radar, truck parts) show bright white specular highlights

### Light Attenuation

When light travels through space, its intensity diminishes with distance.

**Attenuation Formula**:
```
Attenuation = 1.0 / (k_c + k_l × distance + k_q × distance²)
```

Where:
- **k_c** (constant): Base attenuation (usually 1.0)
- **k_l** (linear): First-order distance falloff (usually 0.09)
- **k_q** (quadratic): Second-order distance falloff (usually 0.032)

**Effect**:
- Small k_l, k_q ? light travels far (wide radius)
- Large k_l, k_q ? light dies quickly (narrow radius)

**Example**: Point lights use k_c=1.0, k_l=0.09, k_q=0.032
- At distance 1: Attenuation ? 1.0
- At distance 5: Attenuation ? 0.38
- At distance 10: Attenuation ? 0.11

---

## LIGHT TYPES USED

### 1. Point Lights
**Characteristics**:
- Emits light in all directions from a single point
- Has a position but no direction
- Intensity decreases with distance (attenuation)
- **Real-world analogy**: Light bulb in a room

**Used For**: General ambient illumination, area lighting

**Parameters**:
- Position (x, y, z)
- Ambient, Diffuse, Specular colors
- Attenuation coefficients (k_c, k_l, k_q)

---

### 2. Directional Light
**Characteristics**:
- Simulates sun/moon light from infinitely far away
- All light rays are parallel
- No attenuation (same intensity everywhere)
- Has a direction but no specific position
- **Real-world analogy**: Sunlight

**Used For**: Main scene illumination, creating shadows

**Parameters**:
- Direction vector
- Ambient, Diffuse, Specular colors

---

### 3. Spot Light
**Characteristics**:
- Emits light in a cone from a point
- Has both position and direction
- Creates a focused beam of light
- Attenuation based on distance
- **Real-world analogy**: Flashlight, headlight, stage light

**Used For**: Focused lighting effects like headlights

**Parameters**:
- Position (x, y, z)
- Direction vector
- Cutoff angle (defines cone width)
- Ambient, Diffuse, Specular colors
- Attenuation coefficients

**Cutoff Angle Theory**:
- Measured in degrees
- Defines the half-angle of the light cone
- Smaller angle = narrower, more focused beam
- Larger angle = wider, more diffuse beam

---

## ALL LIGHTS IN SCENE

### CATEGORY 1: AMBIENT BASE POINT LIGHTS (20 lights)
**Purpose**: Create base ambient illumination across the base camp

#### Light Distribution Pattern:
```
                    North
         (NORTHWEST)  |  (NORTHEAST)
              PL4???????????PL2
              ?       ?       ?
              ?   (CENTER)   ?
        PL5??????PL13-PL14??PL3
        PL6??????PL15-PL16??PL1
              ?   (CENTER)   ?
              ?       ?       ?
              PL7???????????PL10
         (SOUTHWEST)  |  (SOUTHEAST)
                    South
```

#### Corner Lights (12 lights):
| Light # | Position | Color (R, G, B) | Purpose |
|---------|----------|-----------------|---------|
| **PL1** | (50, 20, 50) | 0.8, 0.8, 0.8 | Southeast corner |
| **PL2** | (60, 20, -60) | 0.8, 0.8, 0.8 | Northeast corner |
| **PL3** | (70, 20, -50) | 0.8, 0.8, 0.8 | Northeast area |
| **PL4** | (-60, 20, -60) | 0.8, 0.8, 0.8 | Northwest corner |
| **PL5** | (-70, 20, -50) | 0.8, 0.8, 0.8 | Northwest area |
| **PL6** | (-50, 20, -50) | 0.8, 0.8, 0.8 | Northwest area |
| **PL7** | (-70, 20, 50) | 0.8, 0.8, 0.8 | Southwest area |
| **PL8** | (-60, 20, 60) | 0.8, 0.8, 0.8 | Southwest corner |
| **PL9** | (-50, 20, 50) | 0.8, 0.8, 0.8 | Southwest area |
| **PL10** | (50, 20, 50) | 0.8, 0.8, 0.8 | Southeast area |
| **PL11** | (60, 20, 60) | 0.8, 0.8, 0.8 | Southeast corner |
| **PL12** | (70, 20, 50) | 0.8, 0.8, 0.8 | Southeast area |

#### Center Lights (8 lights):
| Light # | Position | Color (R, G, B) | Purpose |
|---------|----------|-----------------|---------|
| **PL13** | (0, 20, 0) | 0.8, 0.8, 0.8 | Center of base |
| **PL14** | (20, 20, 0) | 0.8, 0.8, 0.8 | Center-right |
| **PL15** | (-20, 20, 0) | 0.8, 0.8, 0.8 | Center-left |
| **PL16** | (0, 20, 20) | 0.8, 0.8, 0.8 | Center-front |
| **PL17** | (0, 20, -20) | 0.8, 0.8, 0.8 | Center-back |
| **PL18** | (20, 20, 20) | 0.8, 0.8, 0.8 | Center-right-front |
| **PL19** | (-20, 20, 20) | 0.8, 0.8, 0.8 | Center-left-front |
| **PL20** | (20, 20, -20) | 0.8, 0.8, 0.8 | Center-right-back |

**All Base Point Lights Share**:
- Height: y = 20.0 (high above ground for wide coverage)
- Ambient: 0.05, 0.05, 0.05 (very subtle)
- Diffuse: 0.8, 0.8, 0.8 (strong)
- Specular: 1.0, 1.0, 1.0 (full intensity)
- Attenuation: k_c=1.0, k_l=0.09, k_q=0.032

---

### CATEGORY 2: DIRECTIONAL LIGHT (SUN)
**Name**: `directlight`  
**Type**: Directional Light  
**Purpose**: Main key light, simulates sun illumination  
**Direction**: From position (80, 90, -80) towards origin

#### Properties:
| Property | Value | Purpose |
|----------|-------|---------|
| **Direction** | (80, 90, -80) normalized | Sunlight angle |
| **Ambient** | (0.35, 0.35, 0.32) | Warm, subtle ambient |
| **Diffuse** | (0.95, 0.90, 0.75) | Warm golden tone |
| **Specular** | (1.0, 0.95, 0.85) | Warm highlights |
| **Range** | Infinite | No attenuation |

**Visual Effect**: Creates warm daytime lighting with long shadows

**Key Toggle**: **7**
- Press 7 ? Turn ON (daytime mode)
- Press 7 ? Turn OFF (night mode, only point lights)

---

### CATEGORY 3: TUBE LIGHTS (BARRACK INTERIOR - 3 lights)
**Purpose**: Illuminate interior of barrack building  
**Type**: Point Lights positioned inside barracks  
**Location**: Barrack at position (40, 0.1, -40), scaled 0.66×

#### Individual Tube Lights:

**TubeLight 1 - LEFT**
```
Position (World): (-47.75, 1.6, -45.06)
  ?? Ambient: (1.0, 1.0, 0.95) - very bright
  ?? Diffuse: (1.2, 1.2, 1.1) - bright white
  ?? Specular: (1.0, 1.0, 0.95) - white highlights
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 21
```

**TubeLight 2 - RIGHT**
```
Position (World): (-42.25, 1.6, -45.06)
  ?? Ambient: (1.0, 1.0, 0.95)
  ?? Diffuse: (1.2, 1.2, 1.1)
  ?? Specular: (1.0, 1.0, 0.95)
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 22
```

**TubeLight 3 - CENTER**
```
Position (World): (-45.0, 1.6, -45.06)
  ?? Ambient: (0.6, 0.6, 0.55) - reduced (dimmer center)
  ?? Diffuse: (0.8, 0.8, 0.75) - softer
  ?? Specular: (0.6, 0.6, 0.55) - reduced highlights
  ?? Attenuation: k_c=0.8, k_l=0.09, k_q=0.032
  ?? Light Number: 23
```

**Theory**: Tube lights simulate overhead fluorescent ceiling lights in a confined interior space. The left and right lights are bright to ensure visibility, while the center is dimmed to avoid over-illumination.

---

### CATEGORY 4: GARAGE LAMPS (4 lights)
**Purpose**: Light downward onto garage platforms where jeeps and truck are parked  
**Type**: Point Lights  
**New Feature**: Can be toggled ON/OFF with Key 8

#### GarageLamp 1 - JEEP GARAGE (FRONT LEFT)
```
Position: Garage1.lampLightPosFrontLeft()
  ?? World Position: (-29, 7.2, 19)
  ?? Ambient: (0.2, 0.2, 0.15) - warm gold tone
  ?? Diffuse: (0.9, 0.85, 0.5) - warm golden yellow
  ?? Specular: (0.8, 0.8, 0.6) - soft highlights
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 24
  ?? Purpose: Illuminate left corner of jeep garage
```

#### GarageLamp 2 - JEEP GARAGE (FRONT RIGHT)
```
Position: Garage1.lampLightPosFrontRight()
  ?? World Position: (-15, 7.2, 19)
  ?? Ambient: (0.2, 0.2, 0.15)
  ?? Diffuse: (0.9, 0.85, 0.5)
  ?? Specular: (0.8, 0.8, 0.6)
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 25
  ?? Purpose: Illuminate right corner of jeep garage
```

#### GarageLamp 3 - TRUCK GARAGE (FRONT LEFT)
```
Position: Garage2.lampLightPosFrontLeft()
  ?? World Position: (-58, 7.2, 30)
  ?? Ambient: (0.2, 0.2, 0.15)
  ?? Diffuse: (0.9, 0.85, 0.5)
  ?? Specular: (0.8, 0.8, 0.6)
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 26
  ?? Purpose: Illuminate left corner of truck garage
```

#### GarageLamp 4 - TRUCK GARAGE (FRONT RIGHT)
```
Position: Garage2.lampLightPosFrontRight()
  ?? World Position: (-42, 7.2, 30)
  ?? Ambient: (0.2, 0.2, 0.15)
  ?? Diffuse: (0.9, 0.85, 0.5)
  ?? Specular: (0.8, 0.8, 0.6)
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Light Number: 27
  ?? Purpose: Illuminate right corner of truck garage
```

**Design Theory**: 
- 4 lamps create balanced lighting from corners
- Warm golden color simulates outdoor-style lamp fixtures
- Dynamic positioning ensures lights stay with garage structure
- Positioned at front to light platform where vehicles park

**Key Toggle**: **8**
- Press 8 ? ALL garage lamps ON
- Press 8 ? ALL garage lamps OFF
- Console output: "Garage Lamps: ON" or "Garage Lamps: OFF"

---

### CATEGORY 5: JEEP HEADLIGHT SPOTLIGHTS (4 lights)
**Purpose**: Illuminate terrain ahead when jeep headlights are on  
**Type**: Spot Lights  
**Count**: 2 per jeep × 2 jeeps = 4 spotlights

#### Jeep A - LEFT HEADLIGHT
```
Name: jeepAHeadlightL
  ?? Position: Updated each frame (front-left of jeep)
  ?? Direction: Forward direction of jeep (rotates with jeep)
  ?? Type: Spotlight with cone
  ?? Cutoff Angle: 20.0° (focused beam)
  ?? Color (ON): Diffuse (1.0, 0.95, 0.8) - yellow-white
  ?? Color (OFF): (0.0, 0.0, 0.0) - dark
  ?? Attenuation: k_c=1.0, k_l=0.09, k_q=0.032
  ?? Spotlight Array Index: 0
```

#### Jeep A - RIGHT HEADLIGHT
```
Name: jeepAHeadlightR
  ?? Position: Updated each frame (front-right of jeep)
  ?? Direction: Forward direction of jeep
  ?? Cutoff Angle: 20.0°
  ?? Color (ON): Diffuse (1.0, 0.95, 0.8) - yellow-white
  ?? Color (OFF): (0.0, 0.0, 0.0)
  ?? Spotlight Array Index: 1
```

#### Jeep B - LEFT HEADLIGHT
```
Name: jeepBHeadlightL
  ?? Position: Updated each frame (front-left of jeep B)
  ?? Direction: Forward direction of jeep B
  ?? Cutoff Angle: 20.0°
  ?? Color (ON): Diffuse (1.0, 0.95, 0.8)
  ?? Color (OFF): (0.0, 0.0, 0.0)
  ?? Spotlight Array Index: 2
```

#### Jeep B - RIGHT HEADLIGHT
```
Name: jeepBHeadlightR
  ?? Position: Updated each frame (front-right of jeep B)
  ?? Direction: Forward direction of jeep B
  ?? Cutoff Angle: 20.0°
  ?? Color (ON): Diffuse (1.0, 0.95, 0.8)
  ?? Color (OFF): (0.0, 0.0, 0.0)
  ?? Spotlight Array Index: 3
```

**Spotlight Theory**:
- Each jeep has 2 spotlights for realistic headlight pair
- 20° cutoff creates focused forward-facing beam
- Yellow-white color mimics real vehicle headlights
- Dynamic positioning and direction follow jeep movement
- Only active when jeep is selected AND lights are on

**Key Toggle for Headlights**: **L** (when jeep selected)
- Press L ? Toggle selected jeep's headlights ON/OFF
- Console output: (depends on selection)

**How to Control**:
1. Press **J** to cycle through jeeps (None ? Jeep A ? Jeep B ? None)
2. When jeep selected, press **L** to toggle headlights
3. Headlights illuminate the direction jeep is facing
4. Use arrow keys to turn and move jeep
5. Headlight beam rotates with jeep

---

## KEY CONTROLS

### LIGHTING CONTROL KEYS

| Key | Action | Effect | Console Output |
|-----|--------|--------|-----------------|
| **4** | Toggle Ambient Light | ON/OFF all ambient components | "Ambient: ON/OFF" |
| **5** | Toggle Diffuse Light | ON/OFF all diffuse components | "Diffuse: ON/OFF" |
| **6** | Toggle Specular Light | ON/OFF all specular components | "Specular: ON/OFF" |
| **7** | Toggle Sunlight | DAY/NIGHT mode | "Sunlight: ON/OFF" |
| **8** | Toggle Garage Lamps | All 4 garage lamps ON/OFF | "Garage Lamps: ON/OFF" |
| **L** (when jeep selected) | Toggle Jeep Headlights | Selected jeep lights ON/OFF | (jeep-specific) |

### Light Control Workflow

#### Day/Night Mode
```
Press 7 ? Activates directional sunlight (warm day mode)
Press 7 ? Deactivates sunlight (night mode, only point lights glow)
```

#### Ambient Visibility Control
```
Press 4 ? Remove ambient light for dramatic effect
Press 4 ? Add ambient light back for visibility in shadows
```

#### Headlight System
```
Press J ? Select Jeep A
Press L ? Turn ON Jeep A headlights
  ?? Two spotlights illuminate forward direction
Press L ? Turn OFF Jeep A headlights
```

#### Garage Illumination
```
Press 8 ? Illuminates all 4 garage corner lamps
  ?? Warm golden light on both jeep and truck garages
Press 8 ? Extinguish all garage lamps
```

---

## PLACEMENT COORDINATES

### Overview Map (Top-Down View)
```
                    Y-AXIS (altitude)
                        ?
                        |
    Z-AXIS (depth)      |  Screen View
           ?????????????+????????????? X-AXIS (width)
                        |
                        ?

    Northwest (-70,-50)        Northeast (70,-50)
         ? PL5                      ? PL3
         ?                          ?
    ?PL6??? ? PL4             ? PL2 ???? PL1
         ?                          ?
         ?  ? PL15                  ?
         ?  ? PL16    ? PL13        ?
         ?  ? PL17  ? PL14         ?
         ?                          ?
    ?PL7???? PL8             ? PL10 ???? PL11
         ?                          ?
    Southwest (-70,50)         Southeast (70,50)

    ? = Point Light Position
    All at height Y = 20.0
```

### Detailed Coordinate Reference

#### Base Point Lights (Y = 20.0)
```
Corner Lights:
  PL1  (50, 20, 50)      - Southeast
  PL2  (60, 20, -60)     - Northeast
  PL3  (70, 20, -50)     - Northeast edge
  PL4  (-60, 20, -60)    - Northwest
  PL5  (-70, 20, -50)    - Northwest edge
  PL6  (-50, 20, -50)    - Northwest center
  PL7  (-70, 20, 50)     - Southwest edge
  PL8  (-60, 20, 60)     - Southwest
  PL9  (-50, 20, 50)     - Southwest center
  PL10 (50, 20, 50)      - Southeast center
  PL11 (60, 20, 60)      - Southeast
  PL12 (70, 20, 50)      - Southeast edge

Center Lights:
  PL13 (0, 20, 0)        - Dead center
  PL14 (20, 20, 0)       - Center-right
  PL15 (-20, 20, 0)      - Center-left
  PL16 (0, 20, 20)       - Center-front
  PL17 (0, 20, -20)      - Center-back
  PL18 (20, 20, 20)      - Center-right-front
  PL19 (-20, 20, 20)     - Center-left-front
  PL20 (20, 20, -20)     - Center-right-back
```

#### Directional Light (Sun)
```
Sun Position: (80.0, 90.0, -80.0)
  ?? X: Far right (80 units east)
  ?? Y: High above (90 units up)
  ?? Z: Far back (80 units back)

Direction (normalized towards origin):
  From (80, 90, -80) ? (0, 0, 0)
  Creates diagonal warm lighting
```

#### Tube Lights (Barrack Interior)
```
Barrack Position: (40, 0.1, -40)
Barrack Scale: 0.66×

TubeLight 1 LEFT:  (-47.75, 1.6, -45.06)
TubeLight 2 RIGHT: (-42.25, 1.6, -45.06)
TubeLight 3 CENTER: (-45.0, 1.6, -45.06)

All at height Y = 1.6 (indoor ceiling level)
Positioned inside barrack interior
```

#### Garage Lamps (Dynamic)
```
Jeep Garage 1 Position: (-22, 0.1, 10)
  ?? GarageLamp1 (Front-Left):  (-29, 7.2, 19)
  ?? GarageLamp2 (Front-Right): (-15, 7.2, 19)

Truck Garage 2 Position: (-50, 0.1, 10)
  ?? GarageLamp3 (Front-Left):  (-58, 7.2, 30)
  ?? GarageLamp4 (Front-Right): (-42, 7.2, 30)

Lamps positioned at corners, height Y = 7.2
```

#### Jeep Headlights (Dynamic - Updates Each Frame)
```
Jeep A Position: (-24.6, 1.3, 9.0)
  ?? Headlight L: (-24.6 - 0.6*cos(yaw), 1.77, 9.0 - sin(yaw))
  ?? Headlight R: (-24.6 + 0.6*cos(yaw), 1.77, 9.0 - sin(yaw))

Jeep B Position: (-19.4, 1.3, 9.0)
  ?? Headlight L: (-19.4 - 0.6*cos(yaw), 1.77, 9.0 - sin(yaw))
  ?? Headlight R: (-19.4 + 0.6*cos(yaw), 1.77, 9.0 - sin(yaw))

Formula: Position = Jeep_Center + (offset_left/right) + (forward_offset)
  ?? Left offset: -0.6 units perpendicular to forward
  ?? Right offset: +0.6 units perpendicular to forward
  ?? Forward offset: 1.2 units ahead of jeep
```

---

## TECHNICAL IMPLEMENTATION

### Shader Integration

#### Fragment Shader (fragmentShaderForPhongShading.fs)
```glsl
// Processes all lighting calculations
// Supports:
//   - 20 Point Lights (indices 0-19)
//   - 1 Directional Light
//   - 4 Spot Lights (jeep headlights)
//   - Phong reflection model
```

#### Vertex Shader (vertexShaderForPhongShading.vs)
```glsl
// Passes vertex positions and normals to fragment shader
// Required for all lighting calculations
```

### Light Array Structure

```cpp
// Point Lights (0-19): Base illumination
PointLight pointlight1-20

// Special Point Lights (20-27): Additional lighting
TubeLight tubeLightLeft       // Index 21
TubeLight tubeLightRight      // Index 22
TubeLight tubeLightCenter     // Index 23
PointLight garageLamp1        // Index 24
PointLight garageLamp2        // Index 25
PointLight garageLamp3        // Index 26
PointLight garageLamp4        // Index 27

// Directional Light (Sun)
DirectionLight directlight    // Always active (unless toggled)

// Spot Lights (Jeep Headlights)
SpotLight jeepAHeadlightL     // Array index 0
SpotLight jeepAHeadlightR     // Array index 1
SpotLight jeepBHeadlightL     // Array index 2
SpotLight jeepBHeadlightR     // Array index 3
```

### Runtime Updates

#### Each Frame (main render loop):
1. Update jeep headlight positions and directions
2. Update garage lamp positions (if changed)
3. Set shader uniforms for all active lights
4. Render scene with current lighting

#### Key Press Handling:
- Key 4: Toggle ambient toggle ? affects all point lights
- Key 5: Toggle diffuse toggle ? affects all point lights
- Key 6: Toggle specular toggle ? affects all point lights
- Key 7: Toggle sunlight ? turns directional light on/off
- Key 8: Toggle garage lamps ? turns lamps 24-27 on/off
- Key L (jeep selected): Toggle selected jeep headlights

---

## VIVA QUESTIONS & ANSWERS

### Q1: What is the difference between ambient and diffuse light?

**Answer**:
- **Ambient Light**: Global illumination that equally lights all surfaces regardless of orientation. It has no direction and prevents complete darkness. Formula: `Ambient = Ambient_Light_Color × Material_Ambient_Color`

- **Diffuse Light**: Light that scatters equally in all directions when hitting a surface. It depends on the angle between the surface normal and light direction, following Lambert's Cosine Law. Formula: `Diffuse = Diffuse_Light_Color × Material_Diffuse_Color × max(0, Normal · LightDir)`

**Key Difference**: Ambient has no direction; diffuse depends on surface orientation.

---

### Q2: Explain light attenuation and why it's necessary.

**Answer**: Light attenuation simulates how light intensity decreases with distance, making lighting more realistic.

**Formula**: `Attenuation = 1.0 / (k_c + k_l × distance + k_q × distance²)`

**Why it's necessary**:
- Real light diminishes with distance
- Prevents lights from illuminating entire scene equally
- Creates depth and spatial relationships
- Controls light radius/influence

**Constants used**:
- k_c (constant) = 1.0: Base value, ensures light has minimum reach
- k_l (linear) = 0.09: First-order falloff
- k_q (quadratic) = 0.032: Second-order falloff for realistic dropoff

---

### Q3: What are spot lights and how are they different from point lights?

**Answer**:
- **Point Lights**: Emit light in all directions from a position. No directional control. Used for ambient area lighting.

- **Spot Lights**: Emit light in a cone shape from both a position AND a direction. Have a cutoff angle. Used for focused lighting like headlights or flashlights.

**Key Differences**:
| Feature | Point Light | Spot Light |
|---------|------------|-----------|
| Direction | None | Required |
| Shape | Sphere | Cone |
| Cutoff angle | N/A | Yes (20° in jeep lights) |
| Use case | General illumination | Focused beams |
| Complexity | Simpler | More complex |

---

### Q4: How does the Phong reflection model work?

**Answer**: The Phong model combines three types of light to create realistic material appearance:

**Final Color = Ambient + Diffuse + Specular**

Where:
- **Ambient**: `A_light × A_material` (global light)
- **Diffuse**: `D_light × D_material × max(0, N·L)` (matte surface color)
- **Specular**: `S_light × S_material × pow(max(0, R·V), shininess)` (shiny highlights)

**Parameters**:
- N = Surface normal
- L = Light direction
- V = View direction
- R = Reflected light direction
- shininess = 32-64 for glossy, 8-16 for matte

---

### Q5: Why do we need 20 point lights instead of fewer?

**Answer**:
- **Coverage**: Base camp is ~200×200 units; 20 lights at 20 units height provide uniform illumination
- **Realism**: Multiple light sources create more realistic shadows and gradients
- **Distribution**: 12 corner lights + 8 center lights = even distribution
- **Performance**: Manageable performance while achieving quality lighting
- **Flexibility**: Can toggle individual groups or all together

---

### Q6: What is the purpose of garage lamps?

**Answer**:
Garage lamps provide focused down-lighting for the vehicle parking areas:
- **Practical**: Illuminate where vehicles are parked
- **Aesthetic**: Warm golden tone creates welcoming garage atmosphere
- **Dynamic**: Positioned at garage corners, lights follow garage position
- **Toggleable**: Can be turned on/off (Key 8) for different scenarios
- **Realistic**: Simulates outdoor area lamps commonly found at industrial/military facilities

---

### Q7: How do jeep headlights work and why are they dynamic?

**Answer**:
**How they work**:
- Each jeep has 2 spotlights (left and right headlight)
- Spotlights update every frame to follow jeep position and rotation
- 20° cutoff creates focused forward-facing beam
- Yellow-white color (1.0, 0.95, 0.8) mimics real headlights

**Why dynamic**:
- Jeeps can move and rotate
- Headlights must illuminate the direction jeep is facing
- Position updates: Jeep center + offset for left/right placement
- Direction updates: Follow jeep's forward vector
- This creates realistic "flashlight following camera" effect

**Key Formula**:
```
Headlight Position = Jeep Position + (left/right offset) + (forward offset)
Headlight Direction = Jeep Forward Vector
Cone Angle = 20 degrees (sharp focused beam)
```

---

### Q8: Explain directional light vs point light for outdoor scenes.

**Answer**:
**Directional Light (Sun)**:
- Simulates sun/moon light from infinite distance
- All light rays parallel (same direction everywhere)
- No attenuation (uniform intensity)
- Creates dramatic shadows
- Used as primary key light

**Point Light (ambient)**:
- Simulates light bulbs, fires, glowing objects
- All rays originate from single point
- Intensity decreases with distance (attenuation)
- Creates soft local illumination
- Used for fill and ambient lighting

**In our scene**:
- Sun (directional) provides main illumination and shadow direction
- 20 point lights provide secondary ambient lighting and fill
- Combined = Realistic outdoor military base appearance

---

### Q9: What are the material properties and how do they interact with lights?

**Answer**:
**Material Properties**: Each surface has three color properties:
- **Ambient Material**: How much ambient light is absorbed
- **Diffuse Material**: Base color under diffuse light
- **Specular Material**: Color of reflections/highlights

**Interaction**:
```
Final Color = 
  (Light Ambient × Material Ambient) +
  (Light Diffuse × Material Diffuse × NdotL) +
  (Light Specular × Material Specular × RdotV^shininess)
```

**Example**:
- Wood (matte): Low specular, high diffuse
- Metal (shiny): High specular, moderate diffuse
- Rough texture: Spread specular highlights
- Glossy texture: Sharp specular highlights

---

### Q10: How to optimize lighting performance?

**Answer**:
1. **Limit lights per pixel**: Use deferred rendering for many lights
2. **Use LOD**: Reduce light quality far from camera
3. **Frustum culling**: Don't process lights outside view
4. **Bake static lights**: Pre-compute unchanging lighting
5. **Use simpler models**: Phong instead of PBR when possible
6. **Group lights**: Share calculations where possible
7. **Shadows**: Use shadow maps efficiently

**In our scene**:
- 20 point lights is reasonable for desktop system
- Spotlights only on active jeeps
- Directional light is cheap (no per-pixel attenuation)
- Tube lights are fixed (no movement calculations)

---

## SUMMARY TABLE

### Quick Reference: All Lights

| Light Type | Count | Purpose | Key Toggle | Color |
|-----------|-------|---------|-----------|-------|
| Point Lights | 20 | Base illumination | 4,5,6 | White (0.8,0.8,0.8) |
| Directional Light | 1 | Sun/main key | 7 | Warm (0.95,0.90,0.75) |
| Tube Lights | 3 | Barrack interior | None | White (1.2,1.2,1.1) |
| Garage Lamps | 4 | Vehicle parking | 8 | Golden (0.9,0.85,0.5) |
| Spotlights | 4 | Jeep headlights | L | Yellow (1.0,0.95,0.8) |

**Total Active Lights**: Up to 32 lights (varies by toggles)

---

This documentation provides complete understanding of the lighting system for both practical use and theoretical study.
