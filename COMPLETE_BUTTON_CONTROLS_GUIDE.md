# COMPLETE BUTTON CONTROLS GUIDE
## Military Base Camp Simulator

---

# ?? JEEP CONTROLS
**Selection**: Press **J** to cycle through jeeps
- None ? Jeep A ? Jeep B ? None

Once a jeep is selected, all these buttons work:

## Jeep Movement & Navigation
| Button | Action |
|--------|--------|
| **UP ARROW** | Move jeep FORWARD (up was down - now reversed) |
| **DOWN ARROW** | Move jeep BACKWARD (down was up - now reversed) |
| **LEFT ARROW** | Turn jeep LEFT |
| **RIGHT ARROW** | Turn jeep RIGHT |

## Jeep Actions (When Selected)
| Button | Action | Effect |
|--------|--------|--------|
| **D** | Toggle Door | Open/Close left & right doors (both sides) |
| **W** | Toggle Window | Open/Close windows (can drop glass panel) |
| **L** | Toggle Headlights | Turn lights ON/OFF (spotlight beams forward) |
| **V** | Cycle Camera View | Cycles through 4 camera modes: Back ? Front ? Left ? Right ? Back |

### Camera View Modes (V Key):
1. **Back View** - Camera behind jeep (driver's seat perspective)
2. **Front View** - Camera in front of jeep (looking back at jeep)
3. **Left View** - Camera on left side of jeep
4. **Right View** - Camera on right side of jeep

## Headlight Details (L Key)
- **Position**: On front bumper (left & right)
- **When ON**: Bright yellow-white spotlight beams pointing forward
- **When OFF**: No light emission
- **Animation**: Lights start at 10% forward position, gradually move to full front position
- **Type**: Spotlight with 20° cone angle for focused illumination

---

# ?? TRUCK CONTROLS
**Selection**: Press **P** to select/deselect truck

Once truck is selected:

## Truck Movement & Navigation
| Button | Action |
|--------|--------|
| **UP ARROW** | Move truck FORWARD |
| **DOWN ARROW** | Move truck BACKWARD |
| **LEFT ARROW** | Turn truck LEFT |
| **RIGHT ARROW** | Turn truck RIGHT |

## Truck Actions (When Selected)
| Button | Action | Effect |
|--------|--------|--------|
| **K** (Hold) | Cycle Render Mode | Cycles: Color Only ? Texture Only ? Blended (50/50) ? Color Only |
| **V** | Cycle Camera View | Cycles through 4 camera modes: Back ? Front ? Left ? Right ? Back |

### Truck Render Modes (K Key)
1. **Color Only** - Shows solid army green color (0.43, 0.46, 0.33)
2. **Texture Only** - Shows truck texture with details
3. **Blended** - 50/50 mix of color and texture
4. **Back to Color** - Cycles back to color only

### Camera View Modes (V Key - Same as Jeep)
1. Back view
2. Front view  
3. Left side view
4. Right side view

---

# ?? BARRACK CONTROLS
**No selection needed** - Works globally

| Button | Action | Effect |
|--------|--------|--------|
| **B** | Toggle Door | Open/Close barrack main entrance door |

**Details**:
- Barrack has **1 main door** that swings open/closed
- Door animation is smooth
- Multiple barracks in the base (at X=40, Z=-40 and X=40, Z=-10)
- Inside: 3 tube lights illuminate interior (left, center, right)

---

# ?? BUILD (MILITARY BUILDING) CONTROLS
**No selection needed** - Works globally

## Door & Window Controls
| Button | Action | Effect |
|--------|--------|--------|
| **G** | Toggle Main Door | Open/Close building entrance (90° swing) |
| **H** | Toggle LEFT Window | Open/Close left side window (animated drop/raise) |
| **I** | Toggle RIGHT Window | Open/Close right side window (animated drop/raise) |

### Building Details
- **Location**: X=-30, Y=Ground, Z=-30
- **Dimensions**: 14W × 18D × 6.5H units
- **Main Door (G)**: Wooden door, swings 0° to 90°
- **Left Window (H)**: Animated vertical motion (0 to 1.2 units)
- **Right Window (I)**: Animated vertical motion (0 to 1.2 units)
- **Interior**: 2 beds (left & right), 1 table (center), 1 medical box (on table)

---

# ?? CAMERA & VIEW CONTROLS
**Free Camera Mode** (No vehicle selected)

## Movement (WASD)
| Button | Action |
|--------|--------|
| **W** | Move forward |
| **S** | Move backward |
| **A** | Move left |
| **D** | Move right |

## Camera Rotation (XC, QE)
| Button | Action |
|--------|--------|
| **X** | Pitch camera UP |
| **C** | Pitch camera DOWN |
| **Q** | Yaw camera LEFT |
| **E** | Yaw camera RIGHT |

## Mouse Control
- **Mouse Movement** (all modes): Look around
- **Scroll Wheel** (all modes): Zoom in/out

## Bird's Eye View (O Key)
| Press | View Mode |
|-------|-----------|
| **O** (1st) | Toggle ON - Wide view (entire base visible) |
| **O** (2nd) | Close shot (medium zoom) |
| **O** (3rd) | Very close shot (maximum zoom) |
| **O** (4th) | Turn OFF - Return to normal view |

---

# ?? LIGHTING CONTROLS
**Work in all modes** - No selection needed

| Button | Action | Effect |
|--------|--------|--------|
| **4** | Toggle Ambient Light | ON/OFF all ambient components |
| **5** | Toggle Diffuse Light | ON/OFF all diffuse components |
| **6** | Toggle Specular Light | ON/OFF all specular components |
| **7** | Toggle Sunlight | Switch DAY/NIGHT mode (directional light) |

### Lighting Details
- **4 (Ambient)**: Controls environmental light reflection
- **5 (Diffuse)**: Controls main light color/brightness
- **6 (Specular)**: Controls shiny reflections on surfaces
- **7 (Sun)**: Toggles directional sun light (day=on, night=off)

---

# ?? GROUND & TEXTURE CONTROLS
**Work in all modes** - No selection needed

| Button | Action | Effect |
|--------|--------|--------|
| **M** | Cycle Ground Render | Color Only ? Texture ? Blended (50/50) ? Color Only |

### Ground Render Modes (M Key)
1. **Color Only** - Solid sandy color (0.8, 0.8, 0.7)
2. **Texture** - Detailed texture with 4x tiling
3. **Blended** - 50% color + 50% texture mix
4. **Back to Color** - Cycles to color mode

---

# ?? MISCELLANEOUS CONTROLS
**No selection needed** - Work globally

| Button | Action | Effect |
|--------|--------|--------|
| **T** | Toggle Tank Movement | START/STOP tank moving on road |
| **R** | Toggle Radar Rotation | START/STOP radar dish spinning |
| **U** | Toggle Boundary Gate | OPEN/CLOSE military base entrance gate |
| **ESC** | Exit Application | Quit the program |

### Details
- **T (Tank)**: Animates tank moving back/forth on military road
- **R (Radar)**: Rotates the radar dish continuously
- **U (Gate)**: Opens/closes the perimeter boundary gate (animated)

---

# ?? QUICK REFERENCE TABLE

## Priority Order (Most Common)
1. **J** - Select Jeep
2. **UP/DOWN** - Move selected vehicle
3. **LEFT/RIGHT** - Turn vehicle
4. **D, W, L** - Jeep door, window, lights
5. **V** - Change camera view
6. **K** - Truck texture modes
7. **M** - Ground render mode
8. **O** - Bird's eye view

## All Buttons Summary
```
NAVIGATION:
  J - Select Jeep
  P - Select Truck
  UP/DOWN - Move forward/backward
  LEFT/RIGHT - Turn left/right

JEEP SPECIFIC:
  D - Door
  W - Window
  L - Headlights

TRUCK SPECIFIC:
  K - Render mode

BUILDINGS:
  B - Barrack door
  G - Build main door
  H - Build left window
  I - Build right window

CAMERA:
  W/A/S/D - Move (free camera)
  X/C - Pitch up/down
  Q/E - Yaw left/right
  O - Bird's eye view
  V - Vehicle camera modes

LIGHTING:
  4 - Ambient
  5 - Diffuse
  6 - Specular
  7 - Sunlight

OTHER:
  M - Ground render mode
  T - Tank movement
  R - Radar rotation
  U - Boundary gate
  ESC - Exit
```

---

# ?? RENDER MODE COMPARISON

### Truck Render Modes (K)
| Mode | Appearance | Use Case |
|------|-----------|----------|
| Color Only | Solid olive green | Minimalist view |
| Texture | Full detail | Realistic appearance |
| Blended | Color+Texture mix | Balanced look |

### Ground Render Modes (M)
| Mode | Appearance | Use Case |
|------|-----------|----------|
| Color Only | Solid sandy terrain | Performance |
| Texture | Detailed sandy surface | Realism |
| Blended | Color+Texture mix | Balance |

---

# ?? CAMERA VIEWS EXPLAINED

### Free Camera (No Vehicle Selected)
- Full 360° movement
- Use WASD + mouse for precision control
- Best for exploring base camp

### Jeep/Truck Camera Modes (V Key)
1. **Back** - Follow cam behind vehicle
2. **Front** - Reverse angle (looking at vehicle)
3. **Left** - Side view from left
4. **Right** - Side view from right

### Bird's Eye View (O Key)
- Orthographic top-down perspective
- 4 zoom levels available
- Perfect for planning routes or full base overview

---

# ? HEADLIGHT SYSTEM SUMMARY

**How to Use**:
1. Select jeep with **J**
2. Press **L** to toggle lights
3. Lights turn on with bright yellow-white spotlights
4. Lights follow jeep direction automatically
5. Press **L** again to turn off

**Technical Details**:
- 2 spotlights per jeep (left & right on bumper)
- 20° cone angle (focused beam)
- Position: 0.12 units (10% front) ? 1.2 units (full front)
- Animation takes ~2 seconds to reach full position
- Direction: Always points where jeep faces

---

# ??? COMPLETE CONTROL FLOWCHART

```
START
  ?
SELECT MODE
  ?? JEEP (J)
  ?   ?? Move (UP/DOWN)
  ?   ?? Turn (LEFT/RIGHT)
  ?   ?? Door (D)
  ?   ?? Window (W)
  ?   ?? Lights (L)
  ?   ?? Camera (V)
  ?
  ?? TRUCK (P)
  ?   ?? Move (UP/DOWN)
  ?   ?? Turn (LEFT/RIGHT)
  ?   ?? Render Mode (K)
  ?   ?? Camera (V)
  ?
  ?? FREE CAMERA
      ?? Move (WASD)
      ?? Look (XC + QE)
      ?? Bird's Eye (O)
      ?? Mouse (Movement + Scroll)

GLOBAL CONTROLS
  ?? Buildings (B, G, H, I)
  ?? Lighting (4, 5, 6, 7)
  ?? Ground (M)
  ?? Misc (T, R, U)
  ?? Exit (ESC)
```

---

**Last Updated**: Current Version
**Total Buttons**: 28+ interactive controls
