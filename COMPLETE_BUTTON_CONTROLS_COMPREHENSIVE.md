# ?? COMPLETE BUTTON CONTROLS - COMPREHENSIVE REFERENCE

---

## ?? JEEP CONTROLS

### Selection
**Button**: `J`
- Cycles: **None** ? **Jeep A** ? **Jeep B** ? **None**
- Console shows which jeep is selected

### Movement (When Selected)
| Button | Action |
|--------|--------|
| `UP ARROW` | Move FORWARD |
| `DOWN ARROW` | Move BACKWARD |
| `LEFT ARROW` | Turn LEFT |
| `RIGHT ARROW` | Turn RIGHT |

### Jeep Actions (When Selected)
| Button | Action | Details |
|--------|--------|---------|
| `D` | Toggle Door | Opens/closes both side doors |
| `W` | Toggle Window | Opens/closes windows (glass drops) |
| `L` | Toggle Headlights | Turns spotlights ON/OFF |
| `V` | Cycle Camera | Back ? Front ? Left ? Right |

### ? NO Render Mode
- **Jeeps ALWAYS use texture** (jeep.png)
- No button to change to color-only or blended mode
- Cannot customize appearance like truck

---

## ?? TRUCK CONTROLS

### Selection
**Button**: `P`
- Toggles: **Selected** ? **Not Selected**
- Only one truck in the scene

### Movement (When Selected)
| Button | Action |
|--------|--------|
| `UP ARROW` | Move FORWARD |
| `DOWN ARROW` | Move BACKWARD |
| `LEFT ARROW` | Turn LEFT |
| `RIGHT ARROW` | Turn RIGHT |

### Truck Render Mode ?
**Button**: `K` (Hold and release repeatedly)

Cycles through:
1. **COLOR ONLY**
   - Solid olive green color: (0.43, 0.46, 0.33)
   - No texture, just flat color
   
2. **TEXTURE ONLY**
   - Full truck.png texture applied
   - All details visible
   
3. **BLENDED (50/50)**
   - 50% green color + 50% texture mix
   - Balance between color and details
   
4. **Back to COLOR ONLY**
   - Cycle repeats

### Truck Actions (When Selected)
| Button | Action | Details |
|--------|--------|---------|
| `V` | Cycle Camera | Back ? Front ? Left ? Right |

---

## ?? GROUND RENDER MODE ?

### Selection
**Global** - Works without selecting any vehicle

### Ground Render Mode
**Button**: `M` (Hold and release repeatedly)

Cycles through:
1. **COLOR ONLY**
   - Solid sandy color: (0.8, 0.8, 0.7)
   - Performance-optimized
   
2. **TEXTURE ONLY**
   - Full ff.png texture applied
   - 4x texture tiling
   - Detailed appearance
   
3. **BLENDED (50/50)**
   - 50% sandy color + 50% texture
   - Balanced look
   
4. **Back to COLOR ONLY**
   - Cycle repeats

---

## ??? BUILDING CONTROLS

### Barrack Door (Global)
| Button | Action |
|--------|--------|
| `B` | Toggle door | Opens/closes main entrance |

### Build (Military Building) Doors/Windows (Global)
| Button | Action | Details |
|--------|--------|---------|
| `G` | Toggle Main Door | Swings 0° to 90° |
| `H` | Toggle LEFT Window | Drops/raises on left side |
| `I` | Toggle RIGHT Window | Drops/raises on right side |

---

## ?? CAMERA CONTROLS

### Free Camera Mode (No Vehicle Selected)

#### Movement
| Button | Action |
|--------|--------|
| `W` | Move FORWARD |
| `S` | Move BACKWARD |
| `A` | Move LEFT |
| `D` | Move RIGHT |

#### Rotation
| Button | Action |
|--------|--------|
| `X` | Pitch UP |
| `C` | Pitch DOWN |
| `Q` | Yaw LEFT |
| `E` | Yaw RIGHT |

#### Mouse
- **Movement**: Look around (always works)
- **Scroll Wheel**: Zoom in/out (always works)

### Vehicle Camera Modes
**Button**: `V` (When vehicle is selected)

Cycles through 4 views:
1. **Back** - Behind vehicle
2. **Front** - In front (looking back)
3. **Left** - Left side view
4. **Right** - Right side view

### Bird's Eye View
**Button**: `O` (Global - works anytime)

Cycles through:
1. `O` (1st press) - **Wide View** (entire base visible)
2. `O` (2nd press) - **Close Shot** (medium zoom)
3. `O` (3rd press) - **Very Close** (maximum zoom)
4. `O` (4th press) - **OFF** (back to normal camera)

---

## ?? LIGHTING CONTROLS

### Global Lighting (No selection needed)

| Button | Action | Effect |
|--------|--------|--------|
| `4` | Toggle Ambient | ON/OFF ambient light (shadows) |
| `5` | Toggle Diffuse | ON/OFF diffuse light (color) |
| `6` | Toggle Specular | ON/OFF specular (shiny reflections) |
| `7` | Toggle Sunlight | ON = Day, OFF = Night |
| `8` | Toggle Garage Lamps | ON/OFF all 4 garage lights |

### Jeep Headlights
| Button | Action | Effect |
|--------|--------|--------|
| `L` | Toggle Lights | ON/OFF spotlights (when jeep selected) |

**Details**:
- Yellow-white spotlights point forward
- Position: Front bumper (left & right)
- Animation: 2 seconds to full position
- Attenuation: Realistic falloff

---

## ?? MISCELLANEOUS CONTROLS

| Button | Action | Effect |
|--------|--------|--------|
| `T` | Toggle Tank | START/STOP tank moving |
| `R` | Toggle Radar | START/STOP radar spinning |
| `U` | Toggle Gate | OPEN/CLOSE boundary gate |
| `ESC` | Exit | Quit application |

---

## ?? CONTROL SUMMARY TABLE

| Category | Jeep | Truck | Ground | Global |
|----------|------|-------|--------|--------|
| **Select** | J | P | N/A | - |
| **Move** | ???? | ???? | N/A | - |
| **Render Mode** | ? None | ? K | ? M | - |
| **Camera** | V | V | O | - |
| **Special** | D,W,L | - | - | - |
| **Lighting** | - | - | - | 4,5,6,7,8 |
| **Buildings** | - | - | - | B,G,H,I |
| **Misc** | - | - | - | T,R,U,ESC |

---

## ?? QUICK START

### Play with Jeep
```
1. Press J ? Select Jeep A
2. Press UP ? Drive forward
3. Press LEFT/RIGHT ? Turn
4. Press D ? Open door
5. Press L ? Turn on lights
6. Press V ? Change camera
```

### Play with Truck
```
1. Press P ? Select Truck
2. Press UP ? Drive forward
3. Press K ? Change render mode (Color/Texture/Blended)
4. Press V ? Change camera
```

### Explore Ground Modes
```
1. Deselect all (press J 4 times or P once)
2. Press M ? Change ground appearance
```

### Use Free Camera
```
1. Deselect all vehicles
2. Press WASD ? Move
3. Press X/C/Q/E ? Rotate
4. Press O ? Bird's eye view
5. Mouse ? Always works for looking
```

---

## ?? WHAT HAS RENDER MODES

| Object | Has Render Mode | Button | Notes |
|--------|-----------------|--------|-------|
| **Truck** | ? YES | K | Color/Texture/Blended |
| **Ground** | ? YES | M | Color/Texture/Blended |
| **Jeep** | ? NO | - | Always textured |
| **Barrack** | ? NO | - | Always textured |
| **Buildings** | ? NO | - | Always textured |
| **Trees** | ? NO | - | Always textured |
| **Radar** | ? NO | - | Always textured |

---

## ? FREQUENT QUESTIONS

### Q: How to change jeep from texture to color?
**A**: You can't. Jeeps are always textured. Only truck (K) and ground (M) have render modes.

### Q: How to get a better view of the truck?
**A**: 
- Press V to cycle through 4 camera angles
- Use bird's eye view (O) from above
- Free camera (deselect) + mouse for custom angle

### Q: How to make ground look different?
**A**: Press M to cycle through color, texture, and blended modes.

### Q: How to turn off sunlight?
**A**: Press 7 to toggle sunlight on/off (day/night mode).

### Q: How to see jeep with headlights?
**A**: 
1. Select jeep (J)
2. Press L to turn lights on
3. Press V to get camera angle from front

---

## ?? COMPLETE KEY MAP

```
MOVEMENT & TURNING
  ? UP       ? Move forward (vehicle) or move forward (camera)
  ? DOWN     ? Move backward (vehicle) or move backward (camera)
  ? LEFT     ? Turn left (vehicle) or yaw left (camera)
  ? RIGHT    ? Turn right (vehicle) or yaw right (camera)

VEHICLE SELECTION
  J          ? Select Jeep (cycles A/B)
  P          ? Select Truck
  D          ? Toggle Jeep Door
  W          ? Toggle Jeep Window
  L          ? Toggle Jeep Lights

RENDER MODES
  K          ? Truck: Color/Texture/Blended
  M          ? Ground: Color/Texture/Blended

CAMERA CONTROLS
  V          ? Vehicle: Back/Front/Left/Right views
  O          ? Bird's Eye: 4 zoom levels
  X          ? Pitch UP (free camera)
  C          ? Pitch DOWN (free camera)
  Q          ? Yaw LEFT (free camera)
  E          ? Yaw RIGHT (free camera)
  
  WASD       ? Free camera movement
  Mouse      ? Look around (always available)
  Scroll     ? Zoom (always available)

BUILDINGS & GATES
  B          ? Barrack door toggle
  G          ? Build main door toggle
  H          ? Build left window toggle
  I          ? Build right window toggle
  U          ? Boundary gate toggle

LIGHTING
  4          ? Ambient light ON/OFF
  5          ? Diffuse light ON/OFF
  6          ? Specular light ON/OFF
  7          ? Sunlight ON/OFF (Day/Night)
  8          ? Garage lamps ON/OFF

MISCELLANEOUS
  T          ? Tank movement ON/OFF
  R          ? Radar rotation ON/OFF
  ESC        ? Exit application
```

---

## ?? NOTES

- **Jeeps always use texture** - No render mode button exists
- **Only truck and ground have render modes** - K for truck, M for ground
- **All cameras work with mouse** - Mouse movement works in all modes
- **Lights independent** - Each jeep's lights are separate, can have both on
- **Bird's eye view is global** - Works without selecting vehicles

